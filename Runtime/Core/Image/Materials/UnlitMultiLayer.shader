Shader "UDlib/Multi Layer"
{
    Properties
    {
        _TexLayer0 ("Layer 0", 2D) = "white" {}
        _TexLayer1 ("Layer 1", 2D) = "black" {}
        _TexLayer2 ("Layer 2", 2D) = "black" {}
        _Color ("Color", Color) = (1, 1, 1, 1)
        _Roi ("ROI", Vector) = (0, 0, 1, 1)
    }
    SubShader
    {
        Tags {
			"Queue" = "Transparent"
			"IgnoreProjector" = "True"
			"RenderType" = "Transparent"
			"PreviewType" = "Plane"
		}
        Cull Off
		ZWrite Off
		Blend SrcAlpha OneMinusSrcAlpha

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                fixed4 color : COLOR;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                fixed4 color : COLOR;
                float4 vertex : SV_POSITION;
            };

            sampler2D _TexLayer0;
            sampler2D _TexLayer1;
            sampler2D _TexLayer2;
            fixed4 _Color;
            float4 _Roi;
            float4 _TexLayer0_ST;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _TexLayer0);
                o.color = v.color;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_TexLayer0, i.uv);
                fixed4 col1 = tex2D(_TexLayer1, i.uv);
                fixed4 col2 = tex2D(_TexLayer2, i.uv);
                col = col * (1 - col1.a) + col1;
                col = col * (1 - col2.a) + col2;
                col = col * _Color;
                if (i.uv.x < _Roi.x || i.uv.x > _Roi.z ||
                    i.uv.y < _Roi.y || i.uv.y > _Roi.w)
					col.rgb = col.rgb * 0.3f;
                return col;
            }
            ENDCG
        }
    }
}
