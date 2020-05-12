/**
 * @file    layout.cpp
 * @date    2020/3/31
 * @version 0.2
 * @author  忘焊嘞
 * @brief   每一个layout对象表示一个界面
 * @warning 占用大量内存且运行极慢！
 */
#include "layout.hpp"
#include "gui.hpp"
#include "screen.h"
#include "usage.hpp"
#include <algorithm>
#include <cstring>

#ifdef USE_CHINESE_FONT
extern const SGUI_FONT_RES GB2312_FZXS12;
#define GUI_FONT12x GB2312_FZXS12
#else
#define GUI_FONT12x SGUI_DEFAULT_FONT_12
#endif

LayoutBase *ActiveLayout = (LayoutBase *)&gui_background;

void LayoutBase::GoBack() {
    if (pTree->Parent == nullptr)
        return;
    ActiveLayout = pTree->Parent;
    Repaint();
}

void LayoutBase::Repaint() {
    // CarOLED.fnClear();
    ActiveLayout->Show();
    CarOLED.fnSyncBuffer();
}

void LayoutBase::OnKeyEvent(KeyEvts KEvent) {
#define SWITCH_KEVT(id)                                                        \
    case KeyEvts::id:                                                          \
        Key##id##Push();                                                       \
        break

    switch (KEvent) {
        SWITCH_KEVT(Enter);
        SWITCH_KEVT(Up);
        SWITCH_KEVT(Down);
        SWITCH_KEVT(Left);
        SWITCH_KEVT(Right);
    default:
        break;
    }
    Repaint();
}

////////////////////////////////////////////////////////////////////////////////
#pragma region NoticeLayout

NoticeLayout::NoticeLayout(TreeNode *tree, SGUI_CSZSTR message,
                           const SGUI_BMP_RES &icon)
    : LayoutBase(tree) {
    NoticeObject.cszNoticeText = message;
    NoticeObject.pstIcon = &icon;
    SGUI_Notice_FitArea(&CarOLED, &(NoticeObject.stLayout));
}

void NoticeLayout::Show() {
    SGUI_Notice_Repaint(&CarOLED, &NoticeObject, &SGUI_DEFAULT_FONT_16, 0);
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
#pragma region MenuLayout

void MenuLayout::Show() {
    SGUI_Menu_Repaint(&CarOLED, &MenuObject);
}

MenuLayout::MenuLayout(TreeNode *tree, SGUI_INT cnt, SGUI_ITEMS_ITEM *its)
    : LayoutBase(tree) {
    SGUI_RECT layout = {
        .iX = 38,
        .iY = 6,
        .iWidth = 52,
        .iHeight = 52
    };
    SGUI_Menu_Initialize(&MenuObject, &GUI_FONT12x, &layout, its, cnt);
}

void MenuLayout::KeyDownPush() {
    if (MenuObject.stItems.iSelection < MenuObject.stItems.iCount - 1)
        ++MenuObject.stItems.iSelection;
}

void MenuLayout::KeyUpPush() {
    if (MenuObject.stItems.iSelection > 0)
        --MenuObject.stItems.iSelection;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
#pragma region ListLayout

ListLayout::ListLayout(struct TreeNode *tree, SGUI_ITEMS_ITEM *its,
                       std::string title,
                       std::initializer_list<ListLayout::ListItem> i)
    : LayoutBase(tree), Items(i) {
    for (int i(0); i < Items.size(); ++i)
        its[i].szVariableText = Items[i];
    std::copy(title.begin(), title.end(), ListTitle);
    std::fill_n(ListTitle + title.size(), MAX_CHAR_12x - title.size(), ' ');

    ListObject.stLayout = screen_full;
    ListObject.szTitle = ListTitle;
    // Title and font size must set before initialize list object.
    SGUI_List_Initialize(&ListObject, &GUI_FONT12x, its, Items.size());
}

void ListLayout::Show() {
    std::string cpu_str(
        std::to_string(static_cast<unsigned>(std::log10(cpu_usage_get()))));
    std::copy(cpu_str.begin(), cpu_str.end(),
              ListTitle + MAX_CHAR_12x - cpu_str.size());
    SGUI_List_Repaint(&CarOLED, &ListObject);
}

void ListLayout::KeyDownPush() {
    if (ListObject.stItems.iSelection < ListObject.stItems.iCount - 1)
        ++ListObject.stItems.iSelection;
}

void ListLayout::KeyUpPush() {
    if (ListObject.stItems.iSelection > 0)
        --ListObject.stItems.iSelection;
}

ListLayout::ListItem::ListItem(const char *name) : ItemName(name) {
    std::strcpy(str, name);
}

void ListLayout::ListItem::UpdateValue(std::string value) {
    value.resize(std::min(value.size(), MAX_CHAR_12x - ItemName.size() - 1));
    std::strcpy(str + ItemName.length(), value.c_str());
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
#pragma region CurvelLayout

CurveLayout::CurveLayout(struct TreeNode *tree, std::string title,
                         SGUI_RTGRAPH_CONTROL ctrl)
    : LayoutBase(tree), title(title), control(ctrl) {
    SGUI_RealtimeGraph_Initialize(&object);
}

void CurveLayout::Show() {
    SGUI_CHAR szTextBuffer[16];
    SGUI_RECT stTextDisplayArea = {.iX = 1,
                                   .iY = 1,
                                   .iWidth = RECT_WIDTH(CarOLED.stSize) - 2,
                                   .iHeight = 8};
    SGUI_POINT stInnerPos = {0, 0};

    SGUI_Basic_DrawRectangle(&CarOLED, 0, 0, RECT_WIDTH(CarOLED.stSize),
                             RECT_HEIGHT(CarOLED.stSize), SGUI_COLOR_FRGCLR,
                             SGUI_COLOR_BKGCLR);
    SGUI_Text_DrawText(&CarOLED, title.c_str(), &SGUI_DEFAULT_FONT_8,
                       &stTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
    // Paint value.
    SGUI_Common_IntegerToString(
        object.Data->ValueArray[object.Data->ValueCount - 1], szTextBuffer, 10,
        4, ' ');
    stTextDisplayArea.iX = 1;
    stTextDisplayArea.iY = RECT_HEIGHT(CarOLED.stSize) - 6;
    stTextDisplayArea.iWidth = RECT_WIDTH(CarOLED.stSize) - 2;
    stTextDisplayArea.iHeight = 5;
    SGUI_Text_DrawText(&CarOLED, szTextBuffer, &SGUI_DEFAULT_FONT_MiniNum,
                       &stTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
    SGUI_RealtimeGraph_Repaint(&CarOLED, &object);
}

void CurveLayout::KeyEnterPush() { GoBack(); }

void CurveLayout::AppendValue(SGUI_INT new_val) {
    SGUI_RealtimeGraph_AppendValue(&CarOLED, &object, new_val);
    if (ActiveLayout == this)
        Repaint();
}

#pragma endregion
