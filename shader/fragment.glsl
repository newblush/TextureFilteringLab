#version 410
in vec2 texCoords;
out vec4 color;

uniform sampler2D texSlot;
uniform int filteringMode;

vec4 nearest(vec2 uv, vec2 size) {
    vec2 pixel = uv * size;
    vec2 rounded = round(pixel);
    return texelFetch(texSlot, ivec2(rounded), 0);
}

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

float calcMipmapLevel(vec2 uv, vec2 size) {
    float maxLevel = log2(max(size.x, size.y));
    vec2 dx = dFdx(uv);
    vec2 dy = dFdy(uv);
    float delta_max_sqr = max(dot(dx, dx), dot(dy, dy));
    return min(maxLevel, maxLevel + 0.5 * log2(delta_max_sqr));
}

vec4 bilinearWithMipmap(vec2 uv, vec2 size) {
    float N = max(0, calcMipmapLevel(uv, size));
    int n = int(ceil(N));
    return bilinear(uv, size, n);
}

vec4 trilinear(vec2 uv, vec2 size) {
    float N = max(0, calcMipmapLevel(uv, size));
    int n0 = int(floor(N));
    int n1 = int(ceil(N));
    float delta = N - n0;
    return mix(bilinear(uv, size, n0),
            bilinear(uv, size, n1),
            delta);
}

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

void main() {
    vec2 size = textureSize(texSlot, 0);
    if (filteringMode == 1) {
        color = nearest(texCoords, size);
    }
    else if(filteringMode == 2) {
        color = bilinear(texCoords, size, 0);
    }
    else if(filteringMode == 3) {
        color = bilinearWithMipmap(texCoords, size);
    }
    else if(filteringMode == 4) {
        color = trilinear(texCoords, size);
    }
    else if(filteringMode == 5) {
        color = anisotropic(texCoords, size, 2);
    }
    else if(filteringMode == 6) {
        color = anisotropic(texCoords, size, 4);
    }
    else if(filteringMode == 7) {
        color = anisotropic(texCoords, size, 8);
    }
    else if(filteringMode == 8) {
        color = anisotropic(texCoords, size, 16);
    }
    else {
        color = vec4(0, 0, 0, 0);
    }
    
/*
请在这里实现你的片段着色器
可以看到输入有二维向量texCoords，没错这个就是顶点着色器中代表纹理采样坐标的值
输出为color，代表这个片段的rgba值
除此之外还有代表纹理位置的texSlot（它并非真正的纹理图片，而是一个纹理位置，对应了我们绑定的纹理图片）
还有filteringMode，是一个从1-8的值，你需要根据它来执行不同的过滤操作
1： 最近邻
2： 双线性
3： MipMap+双线性
4： 三线性
5： 2x各向异性
6： 4x各向异性
7： 8x各向异性
8： 16x各向异性
******
现在color = texture(texSlot, texCoords)语句没有做任何过滤操作
你需要实现不同过滤函数并根据当前filteringMode进行选择
以下glsl的内置函数可能对你有些帮助
texelFetch texelSize
你可以查阅相关文档去找到更多必要的函数的用法（主要是数学方面的，如取整和插值）
*/
}
