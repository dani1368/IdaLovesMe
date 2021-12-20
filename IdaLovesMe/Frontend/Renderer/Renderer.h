#pragma once
#include "../../Backend/Globalincludes.h"
#include "../../Frontend/Framework/MenuFramework.h"

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)

enum circle_type { FULL, HALF, QUARTER };
enum text_alignment { LEFT, CENTER, RIGHT };

struct vertex
{
    FLOAT x, y, z, rhw;
    DWORD color;
};

namespace Render {

    namespace Fonts
    {
        extern LPD3DXFONT TabIcons;
        extern LPD3DXFONT LegitTabIcons;
        extern LPD3DXFONT Verdana;
        extern LPD3DXFONT Tahombd;
    }

    class CDraw
    {
    public:
        struct sScreen
        {
            float Width;
            float Height;
            float x_center;
            float y_center;
        } Screen;

        void Sprite(LPDIRECT3DTEXTURE9 Texture, IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, D3DCOLOR Color);

        //=============================================================================================
        void Line(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Pos2, DWORD Color);

        void Rect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, float linewidth, D3DCOLOR Color);
        void FilledRect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, D3DCOLOR color);
        void BorderedRect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, float BorderWidth, D3DCOLOR Color, D3DCOLOR BorderColor);
        //void RoundedRect(float x, float y, float w, float h, float radius, bool smoothing, D3DCOLOR color, D3DCOLOR bcolor);

        void Gradient(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, D3DCOLOR Color, D3DCOLOR OtherColor, bool Vertical = false);
       
        //void Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, D3DCOLOR color);
        //void CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR color);

        void Triangle(IdaLovesMe::Vec2 Top, IdaLovesMe::Vec2 Left, IdaLovesMe::Vec2 Right, D3DCOLOR Color);

        void Text(const char* Text, float X, float Y, int Orientation, LPD3DXFONT Font, bool Bordered, D3DCOLOR Color, IdaLovesMe::Vec2 MaxSize = IdaLovesMe::Vec2(0, 0));
        //=============================================================================================

  
        //=============================================================================================
        void Init(LPDIRECT3DDEVICE9 D3dDevice);
        void CreateObjects();
        void ReleaseObjects();
        void Reset();
        //=============================================================================================
        IDirect3DTexture9* GetBgTexture();
        IdaLovesMe::Vec2   GetTextSize(ID3DXFont* Font, const char * Text);
        LPDIRECT3DDEVICE9   GetD3dDevice();
        //=============================================================================================

    private:
        LPDIRECT3DDEVICE9       m_Device;
        LPD3DXSPRITE            m_Sprite;

        IDirect3DTexture9*      m_BgTexture;

        HANDLE                  m_TabFont;
        HANDLE                  m_LegitTabFont;

        bool Initialized = false;
    };

    extern CDraw* Draw;
}