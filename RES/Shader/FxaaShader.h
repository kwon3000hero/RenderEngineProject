///*============================================================================\n\
//									API PORTING\n\
//============================================================================*/\n\
//#ifndef		FXAA_GLSL_120\n\
//	#define	FXAA_GLSL_120 0\n\
//#endif\n\
//#ifndef		FXAA_GLSL_130\n\
//	#define	FXAA_GLSL_130 0\n\
//#endif\n\
//#ifndef		FXAA_HLSL_3\n\
//	#define	FXAA_HLSL_3 0\n\
//#endif\n\
//#ifndef		FXAA_HLSL_4\n\
//	#define	FXAA_HLSL_4 0\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if FXAA_GLSL_120\n\
//	// Requires,\n\
//	//  #version 120\n\
//	//  #extension GL_EXT_gpu_shader4 : enable\n\
//	#define int2 ivec2\n\
//	#define float2 vec2\n\
//	#define float3 vec3\n\
//	#define float4 vec4\n\
//	#define FxaaBool3 bvec3\n\
//	#define FxaaInt2 ivec2\n\
//	#define FxaaFloat2 vec2\n\
//	#define FxaaFloat3 vec3\n\
//	#define FxaaFloat4 vec4\n\
//	#define FxaaBool2Float(a) mix(0.0, 1.0, (a))\n\
//	#define FxaaPow3(x, y) pow(x, y)\n\
//	#define FxaaSel3(f, t, b) mix((f), (t), (b))\n\
//	#define FxaaTex sampler2D\n\
//	#define FxaaTexLod0(t, p) texture2DLod(t, p, 0.0)\n\
//	#define FxaaTexOff(t, p, o, r) texture2DLodOffset(t, p, 0.0, o)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if FXAA_GLSL_130\n\
//	// Requires '#version 130' or better\n\
//	#define int2 ivec2\n\
//	#define float2 vec2\n\
//	#define float3 vec3\n\
//	#define float4 vec4\n\
//	#define FxaaBool3 bvec3\n\
//	#define FxaaInt2 ivec2\n\
//	#define FxaaFloat2 vec2\n\
//	#define FxaaFloat3 vec3\n\
//	#define FxaaFloat4 vec4\n\
//	#define FxaaBool2Float(a) mix(0.0, 1.0, (a))\n\
//	#define FxaaPow3(x, y) pow(x, y)\n\
//	#define FxaaSel3(f, t, b) mix((f), (t), (b))\n\
//	#define FxaaTex sampler2D\n\
//	#define FxaaTexLod0(t, p) textureLod(t, p, 0.0)\n\
//	#define FxaaTexOff(t, p, o, r) textureLodOffset(t, p, 0.0, o)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if FXAA_HLSL_3\n\
//	#define int2 float2\n\
//	#define FxaaInt2 float2\n\
//	#define FxaaFloat2 float2\n\
//	#define FxaaFloat3 float3\n\
//	#define FxaaFloat4 float4\n\
//	#define FxaaBool2Float(a) (a)\n\
//	#define FxaaPow3(x, y) pow(x, y)\n\
//	#define FxaaSel3(f, t, b) ((f)*(!b) + (t)*(b))\n\
//	#define FxaaTex sampler2D\n\
//	#define FxaaTexLod0(t, p) tex2Dlod(t, float4(p, 0.0, 0.0))\n\
//	#define FxaaTexOff(t, p, o, r) tex2Dlod(t, float4(p + (o * r), 0, 0))\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if FXAA_HLSL_4\n\
//	#define FxaaInt2 int2\n\
//	#define FxaaFloat2 float2\n\
//	#define FxaaFloat3 float3\n\
//	#define FxaaFloat4 float4\n\
//	#define FxaaBool2Float(a) (a)\n\
//	#define FxaaPow3(x, y) pow(x, y)\n\
//	#define FxaaSel3(f, t, b) ((f)*(!b) + (t)*(b))\n\
//	struct FxaaTex { SamplerState smpl; Texture2D tex; };\n\
//	#define FxaaTexLod0(t, p) t.tex.SampleLevel(t.smpl, p, 0.0)\n\
//	#define FxaaTexOff(t, p, o, r) t.tex.SampleLevel(t.smpl, p, 0.0, o)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#define FxaaToFloat3(a) FxaaFloat3((a), (a), (a))\n\
//float4 FxaaTexGrad(FxaaTex tex, float2 pos, float2 grad) {\n\
//	#if FXAA_GLSL_120\n\
//		return texture2DGrad(tex, pos.xy, grad, grad);\n\
//	#endif\n\
//	#if FXAA_GLSL_130\n\
//		return textureGrad(tex, pos.xy, grad, grad);\n\
//	#endif\n\
//	#if FXAA_HLSL_3\n\
//		return tex2Dgrad(tex, pos.xy, grad, grad);\n\
//	#endif\n\
//	#if FXAA_HLSL_4\n\
//		return tex.tex.SampleGrad(tex.smpl, pos.xy, grad, grad);\n\
//	#endif\n\
//}\n\
//\n\
///*============================================================================\n\
//									SRGB KNOBS\n\
//------------------------------------------------------------------------------\n\
//FXAA_SRGB_ROP - Set to 1 when applying FXAA to an sRGB back buffer (DX10/11).\n\
//				This will do the sRGB to linear transform,\n\
//				as ROP will expect linear color from this shader,\n\
//				and this shader works in non-linear color.\n\
//============================================================================*/\n\
//#define FXAA_SRGB_ROP 0\n\
//\n\
///*============================================================================\n\
//								DEBUG KNOBS\n\
//------------------------------------------------------------------------------\n\
//All debug knobs draw FXAA-untouched pixels in FXAA computed luma (monochrome).\n\
//\n\
//FXAA_DEBUG_PASSTHROUGH - Red for pixels which are filtered by FXAA with a\n\
//							yellow tint on sub-pixel aliasing filtered by FXAA.\n\
//FXAA_DEBUG_HORZVERT    - Blue for horizontal edges, gold for vertical edges.\n\
//FXAA_DEBUG_PAIR        - Blue/green for the 2 pixel pair choice.\n\
//FXAA_DEBUG_NEGPOS      - Red/blue for which side of center of span.\n\
//FXAA_DEBUG_OFFSET      - Red/blue for -/+ x, gold/skyblue for -/+ y.\n\
//============================================================================*/\n\
//#ifndef		FXAA_DEBUG_PASSTHROUGH\n\
//	#define	FXAA_DEBUG_PASSTHROUGH	0\n\
//#endif\n\
//#ifndef		FXAA_DEBUG_HORZVERT\n\
//	#define	FXAA_DEBUG_HORZVERT		0\n\
//#endif\n\
//#ifndef		FXAA_DEBUG_PAIR\n\
//	#define	FXAA_DEBUG_PAIR			0\n\
//#endif\n\
//#ifndef		FXAA_DEBUG_NEGPOS\n\
//	#define FXAA_DEBUG_NEGPOS		0\n\
//#endif\n\
//#ifndef		FXAA_DEBUG_OFFSET\n\
//	#define	FXAA_DEBUG_OFFSET		0\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if FXAA_DEBUG_PASSTHROUGH || FXAA_DEBUG_HORZVERT || FXAA_DEBUG_PAIR\n\
//	#define FXAA_DEBUG 1\n\
//#endif\n\
//#if FXAA_DEBUG_NEGPOS || FXAA_DEBUG_OFFSET\n\
//	#define FXAA_DEBUG 1\n\
//#endif\n\
//#ifndef FXAA_DEBUG\n\
//	#define FXAA_DEBUG 0\n\
//#endif\n\
//\n\
///*============================================================================\n\
//								COMPILE-IN KNOBS\n\
//------------------------------------------------------------------------------\n\
//FXAA_PRESET - Choose compile-in knob preset 0-5.\n\
//------------------------------------------------------------------------------\n\
//FXAA_EDGE_THRESHOLD - The minimum amount of local contrast required\n\
//						to apply algorithm.\n\
//						1.0/3.0  - too little\n\
//						1.0/4.0  - good start\n\
//						1.0/8.0  - applies to more edges\n\
//						1.0/16.0 - overkill\n\
//------------------------------------------------------------------------------\n\
//FXAA_EDGE_THRESHOLD_MIN - Trims the algorithm from processing darks.\n\
//							Perf optimization.\n\
//							1.0/32.0 - visible limit (smaller isn't visible)\n\
//							1.0/16.0 - good compromise\n\
//							1.0/12.0 - upper limit (seeing artifacts)\n\
//------------------------------------------------------------------------------\n\
//FXAA_SEARCH_STEPS - Maximum number of search steps for end of span.\n\
//------------------------------------------------------------------------------\n\
//FXAA_SEARCH_ACCELERATION - How much to accelerate search,\n\
//							1 - no acceleration\n\
//							2 - skip by 2 pixels\n\
//							3 - skip by 3 pixels\n\
//							4 - skip by 4 pixels\n\
//------------------------------------------------------------------------------\n\
//FXAA_SEARCH_THRESHOLD - Controls when to stop searching.\n\
//						1.0/4.0 - seems to be the best quality wise\n\
//------------------------------------------------------------------------------\n\
//FXAA_SUBPIX_FASTER - Turn on lower quality but faster subpix path.\n\
//						Not recommended, but used in preset 0.\n\
//------------------------------------------------------------------------------\n\
//FXAA_SUBPIX - Toggle subpix filtering.\n\
//				0 - turn off\n\
//				1 - turn on\n\
//				2 - turn on full (ignores FXAA_SUBPIX_TRIM and CAP)\n\
//------------------------------------------------------------------------------\n\
//FXAA_SUBPIX_TRIM - Controls sub-pixel aliasing removal.\n\
//					1.0/2.0 - low removal\n\
//					1.0/3.0 - medium removal\n\
//					1.0/4.0 - default removal\n\
//					1.0/8.0 - high removal\n\
//					0.0 - complete removal\n\
//------------------------------------------------------------------------------\n\
//FXAA_SUBPIX_CAP - Insures fine detail is not completely removed.\n\
//					This is important for the transition of sub-pixel detail,\n\
//					like fences and wires.\n\
//					3.0/4.0 - default (medium amount of filtering)\n\
//					7.0/8.0 - high amount of filtering\n\
//					1.0 - no capping of sub-pixel aliasing removal\n\
//============================================================================*/\n\
//#ifndef FXAA_PRESET\n\
//	#define FXAA_PRESET 3\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 0)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/4.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/12.0)\n\
//	#define FXAA_SEARCH_STEPS			2\n\
//	#define FXAA_SEARCH_ACCELERATION	4\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			1\n\
//	#define FXAA_SUBPIX_CAP				(2.0/3.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 1)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/8.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/16.0)\n\
//	#define FXAA_SEARCH_STEPS			4\n\
//	#define FXAA_SEARCH_ACCELERATION	3\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			0\n\
//	#define FXAA_SUBPIX_CAP				(3.0/4.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 2)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/8.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/24.0)\n\
//	#define FXAA_SEARCH_STEPS			8\n\
//	#define FXAA_SEARCH_ACCELERATION	2\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			0\n\
//	#define FXAA_SUBPIX_CAP				(3.0/4.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 3)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/8.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/24.0)\n\
//	#define FXAA_SEARCH_STEPS			16\n\
//	#define FXAA_SEARCH_ACCELERATION	1\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			0\n\
//	#define FXAA_SUBPIX_CAP				(3.0/4.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 4)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/8.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/24.0)\n\
//	#define FXAA_SEARCH_STEPS			24\n\
//	#define FXAA_SEARCH_ACCELERATION	1\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			0\n\
//	#define FXAA_SUBPIX_CAP				(3.0/4.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#if (FXAA_PRESET == 5)\n\
//	#define FXAA_EDGE_THRESHOLD			(1.0/8.0)\n\
//	#define FXAA_EDGE_THRESHOLD_MIN		(1.0/24.0)\n\
//	#define FXAA_SEARCH_STEPS			32\n\
//	#define FXAA_SEARCH_ACCELERATION	1\n\
//	#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)\n\
//	#define FXAA_SUBPIX					1\n\
//	#define FXAA_SUBPIX_FASTER			0\n\
//	#define FXAA_SUBPIX_CAP				(3.0/4.0)\n\
//	#define FXAA_SUBPIX_TRIM			(1.0/4.0)\n\
//#endif\n\
///*--------------------------------------------------------------------------*/\n\
//#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))\n\
//\n\
///*============================================================================\n\
//									HELPERS\n\
//============================================================================*/\n\
//// Return the luma, the estimation of luminance from rgb inputs.\n\
//// This approximates luma using one FMA instruction,\n\
//// skipping normalization and tossing out blue.\n\
//// FxaaLuma() will range 0.0 to 2.963210702.\n\
//float FxaaLuma(float3 rgb) {\n\
//	return rgb.y * (0.587/0.299) + rgb.x; }\n\
///*--------------------------------------------------------------------------*/\n\
//float3 FxaaLerp3(float3 a, float3 b, float amountOfA) {\n\
//	return (FxaaToFloat3(-amountOfA) * b) +\n\
//		((a * FxaaToFloat3(amountOfA)) + b); }\n\
///*--------------------------------------------------------------------------*/\n\
//// Support any extra filtering before returning color.\n\
//float3 FxaaFilterReturn(float3 rgb) {\n\
//	#if FXAA_SRGB_ROP\n\
//		// Do sRGB encoded value to linear conversion.\n\
//		return FxaaSel3(\n\
//			rgb * FxaaToFloat3(1.0/12.92),\n\
//			FxaaPow3(\n\
//				rgb * FxaaToFloat3(1.0/1.055) + FxaaToFloat3(0.055/1.055),\n\
//				FxaaToFloat3(2.4)),\n\
//			rgb > FxaaToFloat3(0.04045));\n\
//	#else\n\
//		return rgb;\n\
//	#endif\n\
//}\n\
//\n\
///*============================================================================\n\
//								VERTEX SHADER\n\
//============================================================================*/\n\
//float2 FxaaVertexShader(\n\
//// Both x and y range {-1.0 to 1.0 across screen}.\n\
//float2 inPos) {\n\
//	float2 pos;\n\
//	pos.xy = (inPos.xy * FxaaFloat2(0.5, 0.5)) + FxaaFloat2(0.5, 0.5);\n\
//	return pos; }\n\
//\n\
///*============================================================================\n\
//								PIXEL SHADER\n\
//============================================================================*/\n\
//float3 FxaaPixelShader(\n\
//// Output of FxaaVertexShader interpolated across screen.\n\
////  xy -> actual texture position {0.0 to 1.0}\n\
//float2 pos,\n\
//// Input texture.\n\
//FxaaTex tex,\n\
//// RCPFRAME SHOULD PIXEL SHADER CONSTANTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\
//// {1.0/frameWidth, 1.0/frameHeight}\n\
//float2 rcpFrame) {\n\
//\n\
///*----------------------------------------------------------------------------\n\
//			EARLY EXIT IF LOCAL CONTRAST BELOW EDGE DETECT LIMIT\n\
//------------------------------------------------------------------------------\n\
//Majority of pixels of a typical image do not require filtering,\n\
//often pixels are grouped into blocks which could benefit from early exit\n\
//right at the beginning of the algorithm.\n\
//Given the following neighborhood,\n\
//\n\
//		N\n\
//	W M E\n\
//		S\n\
//\n\
//If the difference in local maximum and minimum luma (contrast 'range')\n\
//is lower than a threshold proportional to the maximum local luma ('rangeMax'),\n\
//then the shader early exits (no visible aliasing).\n\
//This threshold is clamped at a minimum value ('FXAA_EDGE_THRESHOLD_MIN')\n\
//to avoid processing in really dark areas.\n\
//----------------------------------------------------------------------------*/\n\
//	float3 rgbN = FxaaTexOff(tex, pos.xy, FxaaInt2( 0,-1), rcpFrame).xyz;\n\
//	float3 rgbW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 0), rcpFrame).xyz;\n\
//	float3 rgbM = FxaaTexOff(tex, pos.xy, FxaaInt2( 0, 0), rcpFrame).xyz;\n\
//	float3 rgbE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 0), rcpFrame).xyz;\n\
//	float3 rgbS = FxaaTexOff(tex, pos.xy, FxaaInt2( 0, 1), rcpFrame).xyz;\n\
//	float lumaN = FxaaLuma(rgbN);\n\
//	float lumaW = FxaaLuma(rgbW);\n\
//	float lumaM = FxaaLuma(rgbM);\n\
//	float lumaE = FxaaLuma(rgbE);\n\
//	float lumaS = FxaaLuma(rgbS);\n\
//	float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));\n\
//	float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));\n\
//	float range = rangeMax - rangeMin;\n\
//	#if FXAA_DEBUG\n\
//		float lumaO = lumaM / (1.0 + (0.587/0.299));\n\
//	#endif\n\
//	if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD)) {\n\
//		#if FXAA_DEBUG\n\
//			return FxaaFilterReturn(FxaaToFloat3(lumaO));\n\
//		#endif\n\
//		return FxaaFilterReturn(rgbM); }\n\
//	#if FXAA_SUBPIX > 0\n\
//		#if FXAA_SUBPIX_FASTER\n\
//			float3 rgbL = (rgbN + rgbW + rgbE + rgbS + rgbM) *\n\
//				FxaaToFloat3(1.0/5.0);\n\
//		#else\n\
//			float3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;\n\
//		#endif\n\
//	#endif\n\
//";
//static const PLCore::String sEndFXAA_FS_Part2 = "\
///*----------------------------------------------------------------------------\n\
//								COMPUTE LOWPASS\n\
//------------------------------------------------------------------------------\n\
//FXAA computes a local neighborhood lowpass value as follows,\n\
//\n\
//	(N + W + E + S)/4\n\
//\n\
//Then uses the ratio of the contrast range of the lowpass\n\
//and the range found in the early exit check,\n\
//as a sub-pixel aliasing detection filter.\n\
//When FXAA detects sub-pixel aliasing (such as single pixel dots),\n\
//it later blends in 'blendL' amount\n\
//of a lowpass value (computed in the next section) to the final result.\n\
//----------------------------------------------------------------------------*/\n\
//	#if FXAA_SUBPIX != 0\n\
//		float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;\n\
//		float rangeL = abs(lumaL - lumaM);\n\
//	#endif\n\
//	#if FXAA_SUBPIX == 1\n\
//		float blendL = max(0.0,\n\
//			(rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;\n\
//		blendL = min(FXAA_SUBPIX_CAP, blendL);\n\
//	#endif\n\
//	#if FXAA_SUBPIX == 2\n\
//		float blendL = rangeL / range;\n\
//	#endif\n\
//	#if FXAA_DEBUG_PASSTHROUGH\n\
//		#if FXAA_SUBPIX == 0\n\
//			float blendL = 0.0;\n\
//		#endif\n\
//		return FxaaFilterReturn(\n\
//			FxaaFloat3(1.0, blendL/FXAA_SUBPIX_CAP, 0.0));\n\
//	#endif\n\
//\n\
///*----------------------------------------------------------------------------\n\
//					CHOOSE VERTICAL OR HORIZONTAL SEARCH\n\
//------------------------------------------------------------------------------\n\
//FXAA uses the following local neighborhood,\n\
//\n\
//	NW N NE\n\
//	W  M  E\n\
//	SW S SE\n\
//\n\
//To compute an edge amount for both vertical and horizontal directions.\n\
//Note edge detect filters like Sobel fail on single pixel lines through M.\n\
//FXAA takes the weighted average magnitude of the high-pass values\n\
//for rows and columns as an indication of local edge amount.\n\
//\n\
//A lowpass value for anti-sub-pixel-aliasing is computed as\n\
//	(N+W+E+S+M+NW+NE+SW+SE)/9.\n\
//This full box pattern has higher quality than other options.\n\
//\n\
//Note following this block, both vertical and horizontal cases\n\
//flow in parallel (reusing the horizontal variables).\n\
//----------------------------------------------------------------------------*/\n\
//	float3 rgbNW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1,-1), rcpFrame).xyz;\n\
//	float3 rgbNE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1,-1), rcpFrame).xyz;\n\
//	float3 rgbSW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 1), rcpFrame).xyz;\n\
//	float3 rgbSE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 1), rcpFrame).xyz;\n\
//	#if (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0)\n\
//		rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);\n\
//		rgbL *= FxaaToFloat3(1.0/9.0);\n\
//	#endif\n\
//	float lumaNW = FxaaLuma(rgbNW);\n\
//	float lumaNE = FxaaLuma(rgbNE);\n\
//	float lumaSW = FxaaLuma(rgbSW);\n\
//	float lumaSE = FxaaLuma(rgbSE);\n\
//	float edgeVert = \n\
//		abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +\n\
//		abs((0.50 * lumaW ) + (-1.0 * lumaM) + (0.50 * lumaE )) +\n\
//		abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));\n\
//	float edgeHorz = \n\
//		abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +\n\
//		abs((0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS )) +\n\
//		abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));\n\
//	bool horzSpan = edgeHorz >= edgeVert;\n\
//	#if FXAA_DEBUG_HORZVERT\n\
//		if(horzSpan) return FxaaFilterReturn(FxaaFloat3(1.0, 0.75, 0.0));\n\
//		else         return FxaaFilterReturn(FxaaFloat3(0.0, 0.50, 1.0));\n\
//	#endif\n\
//	float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;\n\
//	if(!horzSpan) lumaN = lumaW;\n\
//	if(!horzSpan) lumaS = lumaE;\n\
//	float gradientN = abs(lumaN - lumaM);\n\
//	float gradientS = abs(lumaS - lumaM);\n\
//	lumaN = (lumaN + lumaM) * 0.5;\n\
//	lumaS = (lumaS + lumaM) * 0.5;\n\
//\n\
///*----------------------------------------------------------------------------\n\
//				CHOOSE SIDE OF PIXEL WHERE GRADIENT IS HIGHEST\n\
//------------------------------------------------------------------------------\n\
//This chooses a pixel pair.\n\
//For 'horzSpan == true' this will be a vertical pair,\n\
//\n\
//	[N]     N\n\
//	[M] or [M]\n\
//		S     [S]\n\
//\n\
//Note following this block, both {N,M} and {S,M} cases\n\
//flow in parallel (reusing the {N,M} variables).\n\
//\n\
//This pair of image rows or columns is searched below\n\
//in the positive and negative direction \n\
//until edge status changes \n\
//(or the maximum number of search steps is reached).\n\
//----------------------------------------------------------------------------*/\n\
//	bool pairN = gradientN >= gradientS;\n\
//	#if FXAA_DEBUG_PAIR\n\
//		if(pairN) return FxaaFilterReturn(FxaaFloat3(0.0, 0.0, 1.0));\n\
//		else      return FxaaFilterReturn(FxaaFloat3(0.0, 1.0, 0.0));\n\
//	#endif\n\
//	if(!pairN) lumaN = lumaS;\n\
//	if(!pairN) gradientN = gradientS;\n\
//	if(!pairN) lengthSign *= -1.0;\n\
//	float2 posN;\n\
//	posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);\n\
//	posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);\n\
//\n\
///*----------------------------------------------------------------------------\n\
//							CHOOSE SEARCH LIMITING VALUES\n\
//------------------------------------------------------------------------------\n\
//Search limit (+/- gradientN) is a function of local gradient.\n\
//----------------------------------------------------------------------------*/\n\
//	gradientN *= FXAA_SEARCH_THRESHOLD;\n\
//\n\
///*----------------------------------------------------------------------------\n\
//	SEARCH IN BOTH DIRECTIONS UNTIL FIND LUMA PAIR AVERAGE IS OUT OF RANGE\n\
//------------------------------------------------------------------------------\n\
//This loop searches either in vertical or horizontal directions,\n\
//and in both the negative and positive direction in parallel.\n\
//This loop fusion is faster than searching separately.\n\
//\n\
//The search is accelerated using FXAA_SEARCH_ACCELERATION length box filter\n\
//via anisotropic filtering with specified texture gradients.\n\
//----------------------------------------------------------------------------*/\n\
//	float2 posP = posN;\n\
//	float2 offNP = horzSpan ?\n\
//		FxaaFloat2(rcpFrame.x, 0.0) :\n\
//		FxaaFloat2(0.0f, rcpFrame.y);\n\
//	float lumaEndN = lumaN;\n\
//	float lumaEndP = lumaN;\n\
//	bool doneN = false;\n\
//	bool doneP = false;\n\
//	#if FXAA_SEARCH_ACCELERATION == 1\n\
//		posN += offNP * FxaaFloat2(-1.0, -1.0);\n\
//		posP += offNP * FxaaFloat2( 1.0,  1.0);\n\
//	#endif\n\
//	#if FXAA_SEARCH_ACCELERATION == 2\n\
//		posN += offNP * FxaaFloat2(-1.5, -1.5);\n\
//		posP += offNP * FxaaFloat2( 1.5,  1.5);\n\
//		offNP *= FxaaFloat2(2.0, 2.0);\n\
//	#endif\n\
//	#if FXAA_SEARCH_ACCELERATION == 3\n\
//		posN += offNP * FxaaFloat2(-2.0, -2.0);\n\
//		posP += offNP * FxaaFloat2( 2.0,  2.0);\n\
//		offNP *= FxaaFloat2(3.0, 3.0);\n\
//	#endif\n\
//	#if FXAA_SEARCH_ACCELERATION == 4\n\
//		posN += offNP * FxaaFloat2(-2.5, -2.5);\n\
//		posP += offNP * FxaaFloat2( 2.5,  2.5);\n\
//		offNP *= FxaaFloat2(4.0, 4.0);\n\
//	#endif\n\
//	for(int i = 0; i < FXAA_SEARCH_STEPS; i++) {\n\
//		#if FXAA_SEARCH_ACCELERATION == 1\n\
//			if(!doneN) lumaEndN =\n\
//				FxaaLuma(FxaaTexLod0(tex, posN.xy).xyz);\n\
//			if(!doneP) lumaEndP =\n\
//				FxaaLuma(FxaaTexLod0(tex, posP.xy).xyz);\n\
//		#else\n\
//			if(!doneN) lumaEndN =\n\
//				FxaaLuma(FxaaTexGrad(tex, posN.xy, offNP).xyz);\n\
//			if(!doneP) lumaEndP =\n\
//				FxaaLuma(FxaaTexGrad(tex, posP.xy, offNP).xyz);\n\
//		#endif\n\
//		doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);\n\
//		doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);\n\
//		if(doneN && doneP) break;\n\
//		if(!doneN) posN -= offNP;\n\
//		if(!doneP) posP += offNP; }\n\
//\n\
///*----------------------------------------------------------------------------\n\
//				HANDLE IF CENTER IS ON POSITIVE OR NEGATIVE SIDE\n\
//------------------------------------------------------------------------------\n\
//FXAA uses the pixel's position in the span\n\
//in combination with the values (lumaEnd*) at the ends of the span,\n\
//to determine filtering.\n\
//\n\
//This step computes which side of the span the pixel is on.\n\
//On negative side if dstN < dstP,\n\
//\n\
//		posN        pos                      posP\n\
//		|-----------|------|------------------|\n\
//		|           |      |                  |\n\
//		|<--dstN--->|<---------dstP---------->|\n\
//							|\n\
//					span center\n\
//\n\
//----------------------------------------------------------------------------*/\n\
//	float dstN = horzSpan ? pos.x - posN.x : pos.y - posN.y;\n\
//	float dstP = horzSpan ? posP.x - pos.x : posP.y - pos.y;\n\
//	bool directionN = dstN < dstP;\n\
//	#if FXAA_DEBUG_NEGPOS\n\
//		if(directionN) return FxaaFilterReturn(FxaaFloat3(1.0, 0.0, 0.0));\n\
//		else           return FxaaFilterReturn(FxaaFloat3(0.0, 0.0, 1.0));\n\
//	#endif\n\
//	lumaEndN = directionN ? lumaEndN : lumaEndP;\n\
//\n\
///*----------------------------------------------------------------------------\n\
//			CHECK IF PIXEL IS IN SECTION OF SPAN WHICH GETS NO FILTERING\n\
//------------------------------------------------------------------------------\n\
//If both the pair luma at the end of the span (lumaEndN)\n\
//and middle pixel luma (lumaM)\n\
//are on the same side of the middle pair average luma (lumaN),\n\
//then don't filter.\n\
//\n\
//Cases,\n\
//\n\
//(1.) 'L',\n\
//\n\
//				lumaM\n\
//					|\n\
//					V    XXXXXXXX <- other line averaged\n\
//			XXXXXXX[X]XXXXXXXXXXX <- source pixel line\n\
//		|      .      |\n\
//	--------------------------\n\
//		[ ]xxxxxx[x]xx[X]XXXXXX <- pair average\n\
//	--------------------------\n\
//		^      ^ ^    ^\n\
//		|      | |    |\n\
//		.      |<---->|<---------- no filter region\n\
//		.      | |    |\n\
//		. center |    |\n\
//		.        |  lumaEndN\n\
//		.        |    .\n\
//		.      lumaN  .\n\
//		.             .\n\
//		|<--- span -->|\n\
//\n\
//\n\
//(2.) '^' and '-',\n\
//\n\
//								<- other line averaged\n\
//			XXXXX[X]XXX          <- source pixel line\n\
//			|     |     |\n\
//	--------------------------\n\
//		[ ]xxxx[x]xx[ ]        <- pair average\n\
//	--------------------------\n\
//			|     |     |\n\
//			|<--->|<--->|<---------- filter both sides\n\
//\n\
//\n\
//(3.) 'v' and inverse of '-',\n\
//\n\
//	XXXXXX           XXXXXXXXX <- other line averaged\n\
//	XXXXXXXXXXX[X]XXXXXXXXXXXX <- source pixel line\n\
//			|     |     |\n\
//	--------------------------\n\
//	XXXX[X]xxxx[x]xx[X]XXXXXXX <- pair average\n\
//	--------------------------\n\
//			|     |     |\n\
//			|<--->|<--->|<---------- don't filter both!\n\
//\n\
//\n\
//Note the 'v' case for FXAA requires no filtering.\n\
//This is because the inverse of the '-' case is the 'v'.\n\
//Filtering 'v' case turns open spans like this,\n\
//\n\
//	XXXXXXXXX\n\
//\n\
//Into this (which is not desired),\n\
//\n\
//	x+.   .+x\n\
//	XXXXXXXXX\n\
//\n\
//----------------------------------------------------------------------------*/\n\
//	if(((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0))\n\
//		lengthSign = 0.0;\n\
//\n\
///*----------------------------------------------------------------------------\n\
//				COMPUTE SUB-PIXEL OFFSET AND FILTER SPAN\n\
//------------------------------------------------------------------------------\n\
//FXAA filters using a bilinear texture fetch offset\n\
//from the middle pixel M towards the center of the pair (NM below).\n\
//Maximum filtering will be half way between pair.\n\
//Reminder, at this point in the code,\n\
//the {N,M} pair is also reused for all cases: {S,M}, {W,M}, and {E,M}.\n\
//\n\
//	+-------+\n\
//	|       |    0.5 offset\n\
//	|   N   |     |\n\
//	|       |     V\n\
//	+-------+....---\n\
//	|       |\n\
//	|   M...|....---\n\
//	|       |     ^\n\
//	+-------+     |\n\
//	.       .    0.0 offset\n\
//	.   S   .\n\
//	.       .\n\
//	.........\n\
//\n\
//Position on span is used to compute sub-pixel filter offset using simple ramp,\n\
//\n\
//				posN           posP\n\
//				|\\            |<------- 0.5 pixel offset into pair pixel\n\
//				| \\           |\n\
//				|  \\          |\n\
//	---.......|...\\.........|<------- 0.25 pixel offset into pair pixel\n\
//		^        |   ^\\        |\n\
//		|        |   | \\       |\n\
//		V        |   |  \\      |\n\
//	---.......|===|==========|<------- 0.0 pixel offset (ie M pixel)\n\
//		^        .   |   ^      .\n\
//		|        .  pos  |      .\n\
//		|        .   .   |      .\n\
//		|        .   . center   .\n\
//		|        .   .          .\n\
//		|        |<->|<---------.-------- dstN\n\
//		|        .   .          .\n\
//		|        .   |<-------->|<------- dstP\n\
//		|        .             .\n\
//		|        |<------------>|<------- spanLength\n\
//		|\n\
//	subPixelOffset\n\
//\n\
//----------------------------------------------------------------------------*/\n\
//	float spanLength = (dstP + dstN);\n\
//	dstN = directionN ? dstN : dstP;\n\
//	float subPixelOffset = (0.5 + (dstN * (-1.0/spanLength))) * lengthSign;\n\
//	#if FXAA_DEBUG_OFFSET\n\
//		float ox = horzSpan ? 0.0 : subPixelOffset*2.0/rcpFrame.x;\n\
//		float oy = horzSpan ? subPixelOffset*2.0/rcpFrame.y : 0.0;\n\
//		if(ox < 0.0) return FxaaFilterReturn(\n\
//			FxaaLerp3(FxaaToFloat3(lumaO),\n\
//						FxaaFloat3(1.0, 0.0, 0.0), -ox));\n\
//		if(ox > 0.0) return FxaaFilterReturn(\n\
//			FxaaLerp3(FxaaToFloat3(lumaO),\n\
//						FxaaFloat3(0.0, 0.0, 1.0),  ox));\n\
//		if(oy < 0.0) return FxaaFilterReturn(\n\
//			FxaaLerp3(FxaaToFloat3(lumaO),\n\
//						FxaaFloat3(1.0, 0.6, 0.2), -oy));\n\
//		if(oy > 0.0) return FxaaFilterReturn(\n\
//			FxaaLerp3(FxaaToFloat3(lumaO),\n\
//						FxaaFloat3(0.2, 0.6, 1.0),  oy));\n\
//		return FxaaFilterReturn(FxaaFloat3(lumaO, lumaO, lumaO));\n\
//	#endif\n\
//	float3 rgbF = FxaaTexLod0(tex, FxaaFloat2(\n\
//		pos.x + (horzSpan ? 0.0 : subPixelOffset),\n\
//		pos.y + (horzSpan ? subPixelOffset : 0.0))).xyz;\n\
//	#if FXAA_SUBPIX == 0\n\
//		return FxaaFilterReturn(rgbF);\n\
//	#else\n\
//		return FxaaFilterReturn(FxaaLerp3(rgbL, rgbF, blendL));\n\
//	#endif\n\
//}"
