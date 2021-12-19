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
        CDraw()
        {
            m_VB = NULL;
            m_IB = NULL;
            int FontNr = 0;
        }

        struct sScreen
        {
            float Width;
            float Height;
            float x_center;
            float y_center;
        } Screen;

        void Sprite(LPDIRECT3DTEXTURE9 tex, IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, D3DCOLOR color);

        //=============================================================================================
        void Line(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Pos2, DWORD color);

        void Line2(int x, int y, int x2, int y2, D3DCOLOR color);

        void Rect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, float linewidth, D3DCOLOR color);
        void FilledRect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, D3DCOLOR color);
        void BorderedRect(IdaLovesMe::Vec2 Pos, IdaLovesMe::Vec2 Size, float border_width, D3DCOLOR color, D3DCOLOR color_border);
        void RoundedRect(float x, float y, float w, float h, float radius, bool smoothing, D3DCOLOR color, D3DCOLOR bcolor);
        void Gradient(IdaLovesMe::Vec2 pos, IdaLovesMe::Vec2 size, D3DCOLOR color, D3DCOLOR other_color, bool vertical = false);
       
        void Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, D3DCOLOR color);
        void CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR color);

        void Triangle(IdaLovesMe::Vec2 top, IdaLovesMe::Vec2 bleft, IdaLovesMe::Vec2 bright, D3DCOLOR color);

        void Text(const char* text, float x, float y, int orientation, LPD3DXFONT Font, bool bordered, D3DCOLOR color);
        //=============================================================================================

  
        //=============================================================================================
        void Init(LPDIRECT3DDEVICE9 pDev);
        void CreateObjects();
        void ReleaseObjects();
        void Reset();
        //=============================================================================================
        IDirect3DTexture9* GetBgTexture();
        IdaLovesMe::Vec2   GetTextSize(ID3DXFont* Font, const char * Text);
        //=============================================================================================

    private:
        LPDIRECT3DDEVICE9       m_Device;
        LPD3DXSPRITE            m_Sprite;
        ID3DXLine*              m_Line;
        LPDIRECT3DVERTEXBUFFER9 m_VB;    // Buffer to hold vertices
        LPDIRECT3DINDEXBUFFER9  m_IB;    // Buffer to hold indices

        IDirect3DTexture9*      m_BgTexture;

        HANDLE                  m_TabFont;
        HANDLE                  m_LegitTabFont;

        bool Initialized = false;
    };

    extern CDraw* Draw;
}