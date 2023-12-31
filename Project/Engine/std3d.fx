#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"


//static float3 g_vLightPos = float3(0.f, 0.f, 0.f);
//static float3 g_vLightDir = float3(1.f, -1.f, 1.f);
//
//static float3 g_vLightColor = float3(1.f, 1.f, 1.f);
//static float g_fLightSpecCoeff = 0.3f;
//static float3 g_vLightAmb = float3(0.15f, 0.15f, 0.15f); // 환경광

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
  
};


// Std3DShader
//
// Param
#define SPEC_COEFF saturate(g_float_0) //반사 계수 0~1

#define IS_SKYBOX_ENV g_btexcube_0
#define SKYBOX_ENV_TEX g_cube_0
//환경맵핑을 위한 define 만약 재질이 큐브재질이고 t8번의 텍스쳐에 바인딩이 되어있다면

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
   
    ////////////////////////////////
    //내 정점이 표면 방향
    //로컬에서 Normal 방향을 월드로 이동
    //픽셀 세이더로 각각의 픽셀들이 내 방향을 가지게 보간되서 갈 수 있게
    //output.vWorldNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld)).xyz;
    //output.vView
    ///////////////////////////////
    
    // 로컬에서의 Normal 방향을 월드로 이동    
    //g_matWV는 viewspace까지 구한 행렬
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    
    //내 표면 방향 y축
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    //내 표면에 면과 수직인 부분 x값
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    //내 표면과 tan값과 둘다 수직인 방향 z축
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
    
    
    //로켈에서 normal 방향을 월드로 이동 g_mapWVP내 월드상에 크기, 회전
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
    float3 vViewNormal = _in.vViewNormal;
    
    if(g_btex_0)
    {
        //0번텍스트면 텍스쳐에 색 그대로 샘플링
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    // normaltext
    if(g_btex_1)
    {
        //방향벡터를 가지고있는 normaltex
        float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        
        //0 ~ 1 범위의 값을 -1 ~ 1로 확장
        vNormal = vNormal * 2 - 1.f;
        
        float3x3 vRotateMat =
        {
            //텍스쳐 문제로 z값과 y값을 바꿔야함 다른 그래픽라이브러리 전용 텍스쳐임
            //내 x값 , z값 , y값
            _in.vViewTangent,
            -_in.vViewBinormal,
            _in.vViewNormal
        };

        //내 표면면적 = 텍스쳐에 방향벡터 내 xyz값 
        //텍스쳐 노멀을 내 물체의 표면 벡터에 적용
        vViewNormal = normalize(mul(vNormal, vRotateMat));
    }
    
    tLightColor lightcolor = (tLightColor) 0.f;
    float fSpecPow = 0.f;
    
    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        CalcLight3D(_in.vViewPos, vViewNormal, i, lightcolor, fSpecPow);
    }
    
    vOutColor.xyz = vOutColor.xyz * lightcolor.vDiffuse.xyz
                    + vOutColor.xyz * lightcolor.vAmbient.xyz
                    + saturate(g_Light3DBuffer[0].Color.vDiffuse.xyz) * 0.3f * fSpecPow * SPEC_COEFF;
    
    
    //skybox
    if(IS_SKYBOX_ENV)
    {
        //내 시선벡터(카메라)
        float3 vEye = normalize(_in.vViewPos);
        //내 시선벡터와 표면벡터의 반사벡터
        float3 vEyeReflect = normalize(reflect(vEye, vViewNormal));
        
        //내 시선벡터와 표면벡터의 반사벡터를 view행렬의 역행렬을 취하면 월드에서의 반사벡터가 나옴
        vEyeReflect = normalize(mul(float4(vEyeReflect, 0.f), g_matViewInv));
        
        //반사벡터가 가리키는 스카이 박스의 픽셀로 샘플링(큐브텍스쳐 샘플링 방향벡터에 따른 샘플링)
        vOutColor *= SKYBOX_ENV_TEX.Sample(g_sam_0, vEyeReflect);
    }
    
    return vOutColor;
   
}


//float4 vOutColor = (float4) 0.f;
    
//    vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
    
//    //월드상에서의 광원의 방향
//    g_vLightDir = normalize(g_vLightDir);
    
//    //월드상에서의 노멀벡터와 광원의 방향을 내적(램버프 코사인 법칙)
//    //-값으로 내적의 크기가 커지지 않게
//    float fLightPow = saturate(dot(_in.vWorldNormal, -g_vLightDir));
    
//    //반사광
//    float3 vWorldReflect = g_vLightDir + 2.f * (dot(_in.vWorldNormal, -g_vLightDir)) * _in.vWorldNormal;
    
    
//    vOutColor.xzy = (vOutColor.xyz * fLightPow) + (vOutColor.xyz * g_vLightAmb);
    
//    return vOutColor;


/*
 //viewSpace에서의 광원의 방향
    //내월드에서 빛이 튕겨나가는방향
    float3 vViewLightDir = normalize(mul(float4(normalize(g_vLightDir), 0.f), g_matView)).xyz;
   
    //viewSpace에서의 노말벡터와 광원의 방향을 내적(램버트 코사인 법칙)
    float fLightPow = saturate(dot(vViewNormal, -vViewLightDir));
    
    //반사광
    float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(vViewNormal, -vViewLightDir)) * vViewNormal);
    //카메라가 바라보는 방향 
    float3 vEye = normalize(_in.vViewPos);
    
    //반사광의 세기 구하기
    float fSpecPow = saturate(dot(vViewReflect, -vEye));
    fSpecPow = pow(fSpecPow, 20);
    
    vOutColor.xyz = (vOutColor.xyz * g_vLightColor * fLightPow) +
                    (vOutColor.xyz * g_vLightColor * g_vLightAmb) +
                    g_vLightColor * g_fLightSpecCoeff * fSpecPow;

    return vOutColor;

*/

#endif