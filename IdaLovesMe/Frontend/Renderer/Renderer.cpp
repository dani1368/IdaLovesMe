#include "Renderer.h"
#include "../../Backend/Misc/Fonts/Fonts.h"
#include "../../Backend/Misc/Textures/Textures.h"

Render::CDraw* Render::Draw = new Render::CDraw();

using namespace Render;
using namespace IdaLovesMe;

namespace Render {
    namespace Fonts {
        LPD3DXFONT TabIcons;
        LPD3DXFONT LegitTabIcons;
        LPD3DXFONT Verdana;
        LPD3DXFONT Tahombd;
    }
}

void CDraw::CreateObjects() 
{
    D3DXCreateLine(m_Device, &m_Line);
    D3DXCreateSprite(m_Device, &m_Sprite);

    D3DPRESENT_PARAMETERS pp = {};
    DWORD nFonts = 0;

    D3DXCreateFontA(m_Device, 12, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, "Verdana", &Fonts::Verdana);
    D3DXCreateFontA(m_Device, 12, 0, FW_BOLD, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, FIXED_PITCH, "Tahoma", &Fonts::Tahombd);

    this->m_TabFont = AddFontMemResourceEx((void*)(FontsData::TabIcons), (DWORD)5192, nullptr, &nFonts);
    D3DXCreateFontA(m_Device, 47, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "gamesense", &Fonts::TabIcons);

    this->m_LegitTabFont = AddFontMemResourceEx((void*)(FontsData::TabIcons), (DWORD)5192, nullptr, &nFonts);
    D3DXCreateFontA(m_Device, 32, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, "undefeated", &Fonts::LegitTabIcons);

    D3DXCreateTextureFromFileInMemoryEx(m_Device, TexturesData::BgTexture, 424852, 4096, 4096, D3DX_DEFAULT, NULL,
        pp.BackBufferFormat, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &this->m_BgTexture);
}

void CDraw::ReleaseObjects() 
{
    if (!Initialized)
        return;

    if (m_Line)
        m_Line->Release();
    if (m_Sprite)
        m_Sprite->Release();
    if (Fonts::Verdana)
        Fonts::Verdana->Release();
    if (Fonts::Tahombd)
        Fonts::Tahombd->Release();
    
    if (Fonts::TabIcons) {
        Fonts::TabIcons->Release();
        RemoveFontMemResourceEx(this->m_TabFont); 
    }
    if (Fonts::LegitTabIcons) {
        Fonts::LegitTabIcons->Release();
        RemoveFontMemResourceEx(this->m_LegitTabFont);
    }

    delete Render::Draw;
}

void CDraw::Init(LPDIRECT3DDEVICE9 pDev) 
{
    if (!Initialized) {
        m_Device = pDev;
        CreateObjects();
        Initialized = true;
    }
}

void CDraw::Reset()
{
    D3DVIEWPORT9 screen;
    m_Device->GetViewport(&screen);

    Screen.Width = (float)screen.Width;
    Screen.Height = (float)screen.Height;
    Screen.x_center = Screen.Width / 2;
    Screen.y_center = Screen.Height / 2;

    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void CDraw::Line(Vec2 Pos, Vec2 Pos2, D3DCOLOR color)
{
    D3DXVECTOR2 vLine[2];

    m_Line->SetWidth(1);
    m_Line->SetAntialias(false);
    m_Line->SetGLLines(true);

    vLine[0].x = Pos.x;
    vLine[0].y = Pos.y;
    vLine[1].x = Pos2.x;
    vLine[1].y = Pos2.y;

    m_Line->Begin();
    m_Line->Draw(vLine, 2, color);
    m_Line->End();
}
/*
void CDraw::Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, D3DCOLOR color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == FULL) pi = D3DX_PI;        // Full circle
    if (type == HALF) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == QUARTER) pi = D3DX_PI / 4;   // 1/4 circle

    for (int i = 0; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
        circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x -= x;
        circle[i].y -= y;

        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        circle[i].x = xnew + x;
        circle[i].y = ynew + y;
    }
    /*
    m_Device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_VB, NULL);

    VOID* pVertices;
    m_VB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    m_VB->Unlock();
    

    m_Device->SetTexture(0, NULL);
    m_Device->SetPixelShader(NULL);
    if (smoothing)
    {
        m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    }
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_Device->SetStreamSource(0, m_VB, 0, sizeof(vertex));
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    m_Device->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
    if (m_VB != NULL) m_VB->Release();
    
}

void CDraw::CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi = D3DX_PI;

    if (type == FULL) pi = D3DX_PI;        // Full circle
    if (type == HALF) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == QUARTER) pi = D3DX_PI / 4;   // 1/4 circle

    
    circle[0].x = x;
    circle[0].y = y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].color = color;

    for (int i = 1; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x -= x;
        circle[i].y -= y;

        float xnew = circle[i].x * cos(angle) - circle[i].y * sin(angle);
        float ynew = circle[i].x * sin(angle) + circle[i].y * cos(angle);

        circle[i].x = xnew + x;
        circle[i].y = ynew + y;
    }
    /*
    m_Device->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_VB, NULL);

    VOID* pVertices;
    m_VB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    m_VB->Unlock();
    
    m_Device->SetTexture(0, NULL);
    m_Device->SetPixelShader(NULL);
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_Device->SetStreamSource(0, m_VB, 0, sizeof(vertex));
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
    if (m_VB != NULL) m_VB->Release();
    
}
*/
void CDraw::Rect(Vec2 Pos, Vec2 Size, float linewidth, D3DCOLOR color)
{
    if (linewidth == 0 || linewidth == 1)
    {
        FilledRect(Pos, Vec2(Size.x, 1), color);             // Top
        FilledRect(Vec2(Pos.x, Pos.y + Size.y - 1), Vec2(Size.x, 1), color);         // Bottom
        FilledRect(Vec2(Pos.x, Pos.y + 1), Vec2(1, Size.y - 2 * 1), color);       // Left
        FilledRect(Vec2(Pos.x + Size.x - 1, Pos.y + 1), Vec2(1, Size.y - 2 * 1), color);   // Right
    }
    else
    {
        FilledRect(Pos, Vec2(Size.x, linewidth), color);                                     // Top
        FilledRect(Vec2(Pos.x, Pos.y + Size.y - linewidth), Vec2(Size.x, linewidth), color);                         // Bottom
        FilledRect(Vec2(Pos.x, Pos.y + linewidth), Vec2(linewidth, Size.x - 2 * linewidth), color);               // Left
        FilledRect(Vec2(Pos.x + Size.x - linewidth, Pos.y + linewidth), Vec2(linewidth, Size.y - 2 * linewidth), color);   // Right
    }
}

void CDraw::FilledRect(Vec2 Pos, Vec2 Size, D3DCOLOR color)
{
    vertex vertices[4] = {
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, color },
        { Pos.x, Pos.y, 0.0f, 1.0f, color },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, color },                  
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, color }
    };
    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
}


void CDraw::BorderedRect(Vec2 Pos, Vec2 Size, float border_width, D3DCOLOR color, D3DCOLOR color_border)
{
    FilledRect(Pos, Size, color);
    Rect(Vec2(Pos.x - border_width, Pos.y - border_width), Vec2(Size.x + 2 * border_width, Size.y + border_width), border_width, color_border);
}

/*
void CDraw::RoundedRect(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor)
{
    FilledRect(Vec2(x + radius, y + radius), Vec2(w - 2 * radius - 1, h - 2 * radius - 1), color);   // Center rect.
    FilledRect(Vec2(x + radius, y + 1), Vec2(w - 2 * radius - 1, radius - 1), color);            // Top rect.
    FilledRect(Vec2(x + radius, y + h - radius - 1), Vec2(w - 2 * radius - 1, radius), color);     // Bottom rect.
    FilledRect(Vec2(x + 1, y + radius), Vec2(radius - 1, h - 2 * radius - 1), color);            // Left rect.
    FilledRect(Vec2(x + w - radius - 1, y + radius), Vec2(radius, h - 2 * radius - 1), color);     // Right rect.

    // Smoothing method
    if (smoothing)
    {
        CircleFilled(x + radius, y + radius, radius - 1, 0, QUARTER, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius - 1, 90, QUARTER, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius - 1, 180, QUARTER, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius - 1, 270, QUARTER, 16, color);       // Bottom-left corner

        Circle(x + radius + 1, y + radius + 1, radius, 0, QUARTER, true, 16, bcolor);          // Top-left corner
        Circle(x + w - radius - 2, y + radius + 1, radius, 90, QUARTER, true, 16, bcolor);       // Top-right corner
        Circle(x + w - radius - 2, y + h - radius - 2, radius, 180, QUARTER, true, 16, bcolor);    // Bottom-right corner
        Circle(x + radius + 1, y + h - radius - 2, radius, 270, QUARTER, true, 16, bcolor);      // Bottom-left corner

        Line(Vec2(x + radius, y + 1), Vec2(x + w - radius - 1, y + 1), bcolor);       // Top line
        Line(Vec2(x + radius, y + h - 2), Vec2(x + w - radius - 1, y + h - 2), bcolor);   // Bottom line
        Line(Vec2(x + 1, y + radius), Vec2(x + 1, y + h - radius - 1), bcolor);       // Left line
        Line(Vec2(x + w - 2, y + radius), Vec2(x + w - 2, y + h - radius - 1), bcolor);   // Right line
    }
    else
    {
        CircleFilled(x + radius, y + radius, radius, 0, QUARTER, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius, 90, QUARTER, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius, 180, QUARTER, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius, 270, QUARTER, 16, color);       // Bottom-left corner
    }
}
*/
void CDraw::Gradient(Vec2 Pos, Vec2 Size, D3DCOLOR color, D3DCOLOR other_color, bool vertical) {

    vertex vertices[4] = {
        { Pos.x, Pos.y, 0.0f, 1.0f, color },
        { Pos.x + Size.x, Pos.y, 0.0f, 1.0f, vertical ? color : other_color },
        { Pos.x, Pos.y + Size.y, 0.0f, 1.0f, vertical ? other_color : color },
        { Pos.x + Size.x, Pos.y + Size.y, 0.0f, 1.0f, other_color }
    };

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
}

void CDraw::Text(const char* text, float x_, float y_, int orientation, LPD3DXFONT Font, bool bordered, D3DCOLOR color)
{
    int x( (int)x_ ), y( (int)y_ );
    RECT rect;
    D3DCOLOR bcolor = D3DCOLOR_RGBA(0, 0, 0, get_a(color));
    switch (orientation)
    {
    case LEFT:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        Font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
        break;
    case CENTER:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        Font->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
        break;
    case RIGHT:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            Font->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            Font->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        Font->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
        break;
    }
}

void CDraw::Sprite(LPDIRECT3DTEXTURE9 tex, Vec2 Pos, Vec2 Size, D3DCOLOR color)
{
    D3DXVECTOR3 pos = D3DXVECTOR3(Pos.x, Pos.y, 0.0f);
    RECT DrawArea = {(LONG)pos.x, (LONG)pos.y, LONG(pos.x + Size.x), LONG(pos.y + Size.y)};
    
    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    m_Device->SetPixelShader(NULL);
    m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
    m_Sprite->Draw(tex, &DrawArea, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, get_a(color)));
    m_Sprite->End();
}

void CDraw::Triangle(Vec2 top, Vec2 bleft, Vec2 bright, D3DCOLOR color)
{
    vertex vertices[3] =
    {
        { top.x, top.y, 0.0f, 1.0f, color, },
        { bright.x, bright.y, 0.0f, 1.0f, color, },
        { bleft.x, bleft.y, 0.0f, 1.0f, color, },
    };

    m_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_Device->SetTexture(0, nullptr);
    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

    m_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(vertex));

    m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    m_Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

IDirect3DTexture9* CDraw::GetBgTexture() 
{
    return this->m_BgTexture;
}

Vec2 CDraw::GetTextSize(ID3DXFont* Font, const char* Text) 
{
    auto rect = RECT();
    Font->DrawTextA(0, Text, strlen(Text), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
   
    return Vec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

LPDIRECT3DDEVICE9 CDraw::GetD3dDevice() 
{
    return this->m_Device;
}