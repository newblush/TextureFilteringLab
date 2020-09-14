# 说明文档

要求全部实现，仅更改了两个着色器的实现代码

## vertex.glsl

```glsl
	gl_Position = projection * view * model * vec4(attr0 , 1.0);
    texCoords = attr1;
```

实现了gl_Postion和texCoords的赋值

## fragment.glsl

main函数中根据filteringMode调用不同的过滤函数

### nearest

```glsl
vec4 nearest(vec2 uv, vec2 size) {
    vec2 pixel = uv * size;
    vec2 rounded = round(pixel);
    return texelFetch(texSlot, ivec2(rounded), 0);
}
```

nearest过滤，计算对应texel位置并四舍五入

### bilinear

```glsl
vec4 bilinear(vec2 uv, vec2 size, int n) {
    size *= pow(0.5, n);
    vec2 pixel = uv * size;
    vec2 uv0 = floor(pixel);
    vec2 uv1 = ceil(pixel);
    vec2 delta = (uv - uv0) / size;
    vec4 u0v0 = texelFetch(texSlot, ivec2(uv0.x, uv0.y), n);
    vec4 u1v0 = texelFetch(texSlot, ivec2(uv1.x, uv0.y), n);
    vec4 u0v1 = texelFetch(texSlot, ivec2(uv0.x, uv1.y), n);
    vec4 u1v1 = texelFetch(texSlot, ivec2(uv1.x, uv1.y), n);
    return mix(mix(u0v0, u1v0, delta.x),
            mix(u0v1, u1v1, delta.x),
            delta.y);
}
```

为了方便后面代码复用，加入了参数n代表mipmap level

先计算了该mipmap level下的纹理实际大小，后根据给出的公式计算

### bilinear w. mipmap

```glsl
float calcMipmapLevel(vec2 uv, vec2 size) {
    float maxLevel = log2(max(size.x, size.y));
    vec2 dx = dFdx(uv);
    vec2 dy = dFdy(uv);
    float delta_max_sqr = max(dot(dx, dx), dot(dy, dy));
    return min(maxLevel, maxLevel + 0.5 * log2(delta_max_sqr));
}
```

计算mipmap level（公式中的N）的函数

其中把log2(sqrt(..))简化成了0.5*log2(..)

```glsl
vec4 bilinearWithMipmap(vec2 uv, vec2 size) {
    float N = max(0, calcMipmapLevel(uv, size));
    int n = int(ceil(N));
    return bilinear(uv, size, n);
}
```

N应大于等于0

调用了bilinear函数

### trilinear

```glsl
vec4 trilinear(vec2 uv, vec2 size) {
    float N = max(0, calcMipmapLevel(uv, size));
    int n0 = int(floor(N));
    int n1 = int(ceil(N));
    float delta = N - n0;
    return mix(bilinear(uv, size, n0),
            bilinear(uv, size, n1),
            delta);
}
```

基本与bilinear w. mipmap思路相同

### anisotropic

```glsl
vec4 anisotropic(vec2 uv, vec2 size, int lambda_max) {
    vec2 dx = dFdx(uv);
    vec2 dy = dFdy(uv);
    float px = sqrt(dot(dx, dx));
    float py = sqrt(dot(dy, dy));
    vec2 pmax = px > py ? dx : dy;
    vec2 pmin = px < py ? dx : dy;
    float lambda = min (sqrt(dot(pmax, pmax)) / sqrt(dot(pmin, pmin)), lambda_max);
    lambda = ceil(lambda);
    if (lambda == 1) {
        return trilinear(uv, size);
    }
    else {
        float N = sqrt(dot(pmax, pmax)) / lambda;
        int n0 = int(floor(N));
        int n1 = int(ceil(N));
        float delta = N - n0;
        vec4 sn0 = vec4(0, 0, 0, 0);
        for (int i = 0; i <= lambda - 1; i++) {
            sn0 += bilinear(uv + (i / (lambda - 1) - 0.5) * pmax, size, n0);
        }
        sn0 /= lambda;
        vec4 sn1 = vec4(0, 0, 0, 0);
        for (int i = 0; i <= lambda - 1; i++) {
            sn1 += bilinear(uv + (i / (lambda - 1) - 0.5) * pmax, size, n1);
        }
        sn1 /= lambda;
        return mix(sn0, sn1, delta);
    }
}
```

lambda_max为自定的最大各项异性度

在计算各向异性度时做了向上取整，否则后面的累计求和再平均会出错

在各向异性度为1时退化为trilinear过滤