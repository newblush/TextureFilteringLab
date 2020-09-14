#version 410
layout (location = 0) in vec3 attr0;
layout (location = 1) in vec2 attr1;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * model * vec4(attr0 , 1.0);
    texCoords = attr1;
/*
请在这里实现顶点着色器
可以看到顶点着色器的输入包括：
三维向量vec3 attr0
二维向量vec2 attr1
请根据在SpriteRenderer.cpp里定义的VertexAttribPointer辨识它们分别存储了什么样信息
除此之外还有3个由CPU传入GPU的全局变量，均为4*4的矩阵
model为模型空间->世界空间变换矩阵
projection为观察空间->剪裁空间变换矩阵
view为世界空间->观察空间变换矩阵
如果有兴趣可以在SpriteRenderer.cpp和TextureFilteringScene.cpp里看到它们是如何被计算和传入着色起的
******
你需要在顶点着色器中实现：
1 gl_Postion的赋值，这是一个built-in的变量，用于管线的后续工作，它代表顶点在*剪裁空间*的位置
2 texCoords的赋值，这个一个二维向量，代表该顶点对应的纹理采样的坐标
如果你正确实现后运行程序便应该能看到黑白格子的“平台”场景了
******
tips:
1 glsl语法里的vec系列操作极为灵活，如vec2 2d = attr0.xy或vec4 4d = vec4(attr0.xyz, 0.0)
当然.xyzw和.rgba等等是一样的
2 4*4的矩阵代表某种变换，但只能与4维向量相乘
*/
}