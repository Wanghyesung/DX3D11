#ifndef _STD3D_DEFFERED
#define _STD3D_DEFERRED

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
        
    float3 vViewPos : POSITION;
    
    float3 vViewNormal : NORMAL;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    
    float vDepth : DEPTH;
};

// ===============
// Std3D_Deferred
// DOMAIN : Deferred
// MRT    : DEFERRED MRT
// Rasterizer State     : CULL_BACK
// DepthStencil State   : LESS
// Blend State          : Default

// Parameter
#define     SpecCoeff    g_float_0
#define     bActiveColor g_int_1
// ===============


VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    // 로컬에서의 Normal 방향을 월드로 이동      
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
               
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vDepth = output.vPosition.z / output.vPosition.w;
    //깊이
    return output;
}


struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float vEmissive : SV_Target3;
    float4 vData : SV_Target4;
    //vData.x = 빛의 영향을 받을지
    //vData.y = 깊이값 체크
    
};

PS_OUT PS_Std3D_Deferred(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
   
    output.vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    float3 vViewNormal = _in.vViewNormal;
    
    if (g_btex_0)
    {
        output.vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        //imgui에서 a==0이면 그리지 않음 
        //output.vDiffuse.a = 1.f;
    }
    
    //if (g_btex_1)
    //{
    //
    //    float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    //    
    //    // 0 ~ 1 범위의 값을 -1 ~ 1 로 확장        
    //    vNormal = vNormal * 2.f - 1.f;
    //    
    //    float3x3 vRotateMat =
    //    {
    //        _in.vViewTangent,
    //        -_in.vViewBinormal,
    //        _in.vViewNormal        
    //    };
    //    
    //    vViewNormal = normalize(mul(vNormal, vRotateMat));
    //}
    
    output.vNormal = float4(vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    
    //1번째 속성에 빛을 받을지 안받을지 결정
    output.vData = float4(1.f, 0.f, 0.f, 1.f);
    if (bActiveColor)
    {
        output.vData.x = 1.f;
    }
    output.vData.y = _in.vDepth;
   
    //재질계수
    output.vColor.a = saturate(SpecCoeff);
    
    return output;
}

#endif