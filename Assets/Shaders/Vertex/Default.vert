
//Should have per object, per shader, and per frame.
cbuffer ConstantBuffer
{
    //float3 Translation;
    float4x4 WorldViewProjection;
    //float4 Color;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VSMain(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
    //position.w = 1;
    output.position = mul(position, WorldViewProjection);
    output.color = color;
    // output.color.r *= Color.r;
    // output.color.g *= Color.g;
    // output.color.b *= Color.b;
    // output.color.a *= Color.a;
    
    return output;
}