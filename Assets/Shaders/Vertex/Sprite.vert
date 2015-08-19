
//Should have per object, per shader, and per frame.
cbuffer ConstantBuffer
{
    float4x4 WorldViewProjection;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;    // texture coordinates
};

VOut VSMain(float4 position : POSITION, float4 color : COLOR, float2 uv : UV)
{
    VOut output;
    //position.w = 1;
    output.position = mul(position, WorldViewProjection);
    output.color = color;
    output.uv = uv;
    // output.color.r *= Color.r;
    // output.color.g *= Color.g;
    // output.color.b *= Color.b;
    // output.color.a *= Color.a;
    
    return output;
}