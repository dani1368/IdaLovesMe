#pragma once
#include "../../Globalincludes.h"
#include "../../ValveSDK/Vector.h"
#include <memory>

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)


namespace CSRender
{
    enum circle_type { FULL, HALF, QUARTER };
    enum text_alignment { LEFT, CENTER, RIGHT };

    struct vertex
    {
        FLOAT x, y, z, rhw;
        DWORD color;
    };

    namespace Fonts
    {
        extern LPD3DXFONT Verdana;
        extern LPD3DXFONT Tahombd;
        extern LPD3DXFONT SmallFont;
    }

    class CRender
    {
    public:
        struct sScreen
        {
            float Width;
            float Height;
            float x_center;
            float y_center;
        } Screen;

        void Sprite(LPDIRECT3DTEXTURE9 Texture, Vector Pos, Vector Size, D3DCOLOR Color);

        //=============================================================================================
        void Line(Vector Pos, Vector Pos2, D3DCOLOR Color);

        void Rect(Vector Pos, Vector Size, float linewidth, D3DCOLOR Color, bool Antialias = false);
        void FilledRect(Vector Pos, Vector Size, D3DCOLOR color, bool Antialias = false);
        void BorderedRect(Vector Pos, Vector Size, float BorderWidth, D3DCOLOR Color, D3DCOLOR BorderColor);
        void Gradient(Vector Pos, Vector Size, D3DCOLOR LColor, D3DCOLOR ROtherColor, bool Vertical = false, bool Antialias = false);
        void Triangle(Vector Top, Vector Left, Vector Right, D3DCOLOR Color, bool antialias = false);
        void Text(const char* Text, float X, float Y, int Orientation, LPD3DXFONT Font, bool Bordered, D3DCOLOR Color, Vector MaxSize = Vector(0, 0));
        //=============================================================================================


        //=============================================================================================
        void Init(LPDIRECT3DDEVICE9 D3dDevice);
        void CreateObjects();
        void ReleaseObjects();
        void Reset();
        //=============================================================================================
        IDirect3DTexture9* GetBgTexture();
        Vector   GetTextSize(ID3DXFont* Font, const char* Text);
        LPDIRECT3DDEVICE9   GetD3dDevice();
        //=============================================================================================

    private:
        LPDIRECT3DDEVICE9       m_Device;
        LPD3DXSPRITE            m_Sprite;

        IDirect3DTexture9* m_BgTexture;

        HANDLE                  m_TabFont;
        HANDLE                  m_LegitTabFont;

        bool Initialized = false;
    };

    extern std::unique_ptr<CRender> CSDraw;
}