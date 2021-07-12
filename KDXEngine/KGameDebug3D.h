#pragma once
#include "KTransform.h"
#include "KCamera.h"
#include <vector>
#include "KRenderer.h"

enum class KDEBUG_DRAW_TYPE
{
	KDT_RECT,
	KDT_BOX,
	KDT_LINE,
	KDT_CIRCLE,
};

class KVector;
class KRenderTarget;
class KGameFont;
class KGameDebug3D
{
private:
	class KDebugInfo
	{
	public:
		KDEBUG_DRAW_TYPE m_Type;
		KWeakPTR<KCamera> Cam;
		KWeakPTR<KTransform> Trans;
		KVector Color;
	};

	class KDebugTextInfo
	{
	public:
		KGameString Text;
		float Size;
		KVector Pos;
		KVector Color;
	};

	class KTextureInfo
	{
	public:
		KPTR<KTexture> Texture;
		KVector Size;
		KVector Pos;
		KVector BackColor;
	};

private:
	class STATICINIT
	{
	public:
		STATICINIT();
	};
	friend STATICINIT;

public:
	static void Init();
	static bool Debug;
	static void DebugOn()
	{
		Debug = true;
	}

	static void DebugOff()
	{
		Debug = false;
	}

private:
	static float m_basicSize;

public:

	static KPTR<KMesh> RectMesh;
	static KRenderer RP;
	static int DebugSize;
	static std::vector<KDebugInfo> m_Info;

	static int m_DebugTextSize;
	static std::vector<KDebugTextInfo> m_textInfo;

private:
	static KPTR<KRenderer> m_TextureRenderPlayer;
	static KPTR<KRenderer> m_BackbufferRenderPlayer;
	static int m_DebugTextureSize;
	static TransformMatrix m_DebugTextureMatrix;
	static std::vector<KTextureInfo> m_TextureInfo;
	static KTextureInfo m_FullInfo;
	static KVector m_CenterVector;
	static KVector m_BackColor;

public:
	static void DebugText(const KGameString& _str, float _size, const KVector& _pos, const KVector& _color);
	static void DebugDraw(KDEBUG_DRAW_TYPE _Type, KPTR<KTransform> _Ptr, KPTR<KCamera> _Cam);
	static void DebugRect(KPTR<KTransform> _Ptr, KPTR<KCamera> _Cam = static_cast<KPTR<KCamera>>(0));

	static void DebugTexture(KPTR<KRenderTarget> _renderTarget, const KVector& _pos, const KVector& _size, const KVector& _color = KVector::ZERO);
	static void DebugTexture(KPTR<KTexture> _renderTarget, const KVector& _pos, const KVector& _size, const KVector& _color = KVector::ZERO);
	static void DebugFullTexture(KPTR<KTexture> _renderTarget, const KVector& _pos, const KVector& _size, const KVector& _color = KVector::ZERO);


	template<typename ... Args>
	static void DebugText(const KVector& _pos, float _size, const KVector& _color, const KGameString& _str, Args ... _arg)
	{
#ifdef _DEBUG
		wchar_t text[256];
		swprintf_s(text, _str, _arg...);
		KGameDebug3D::DebugText(text, _size, _pos, _color);
#endif
	}
	template<typename ... Args>
	static void ConsoleText(const KGameString& _str, Args ... _arg)
	{
		KGameDebug::ConsoleText(_str, _arg...);
	}

	static void OpenConsole()
	{
		KGameDebug::ConsoleOn();
	}

	static void DebugRender();

};

