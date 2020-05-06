#ifndef LAYOUT_H
#define LAYOUT_H

extern "C" {
#include "SGUI_Basic.h"
#include "SGUI_List.h"
#include "SGUI_Menu.h"
#include "SGUI_Notice.h"
#include "SGUI_ProcessBar.h"
#include "SGUI_RealtimeGraph.h"
}
#include "screen.h"
#include "tree.hpp"
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

#define MAX_CHAR_12x 20

struct LayoutBase {
    enum class KeyEvts { Enter, Up, Down, Left, Right };

    LayoutBase(struct TreeNode *tree) : pTree(tree) {}
    ~LayoutBase() {}
    static void Repaint();
    void GoBack();
    void OnKeyEvent(KeyEvts key_evt);

  protected:
    struct TreeNode *pTree;
    virtual void Show() = 0;
    virtual void KeyEnterPush() {}
    virtual void KeyUpPush() {}
    virtual void KeyDownPush() {}
    virtual void KeyLeftPush() {}
    virtual void KeyRightPush() {}
};

extern LayoutBase *ActiveLayout;

struct BasicLayout : LayoutBase {
    BasicLayout(struct TreeNode *tree) : LayoutBase(tree) {}
};

struct NoticeLayout : LayoutBase {
    NoticeLayout(struct TreeNode *tree, SGUI_CSZSTR message,
                 const SGUI_BMP_RES &icon);

  protected:
    SGUI_NOTICT_BOX NoticeObject = {0x0};
    virtual void Show();
};

struct MenuLayout : LayoutBase {
    MenuLayout(struct TreeNode *tree, SGUI_INT cnt, SGUI_ITEMS_ITEM *its);
    virtual void Show();

  protected:
    SGUI_MENU_STRUCT MenuObject = {0x0};
};

struct ListLayout : LayoutBase {
    class ListItem {
        std::string ItemName, ItemValue;
        char str[MAX_CHAR_12x] = {0x0};

      public:
        ListItem(const char *name);
        operator char *() { return str; }
        void UpdateValue(std::string value);
    };

    ListLayout(struct TreeNode *tree, SGUI_ITEMS_ITEM *its, std::string title,
               std::initializer_list<ListLayout::ListItem> i);

  protected:
    std::vector<ListItem> Items;
    char ListTitle[MAX_CHAR_12x + 1] = "abcdefghijklmnopqrst";
    SGUI_LIST_STRUCT ListObject = {0x0};

    virtual void Show();
    virtual void KeyDownPush();
    virtual void KeyUpPush();
};

struct CurveLayout : LayoutBase {
    CurveLayout(struct TreeNode *tree, std::string title,
                SGUI_RTGRAPH_CONTROL ctrl = {100, 0, SGUI_TRUE, 1, 0});
    void AppendValue(SGUI_INT new_val);
    virtual void Show();

  private:
    const std::string title;
    SGUI_RTGRAPH_CONTROL control;
    SGUI_RTGRAPH_DATA data = {{0}, {0}, {0}, 0, 0};
    SGUI_RTGRAPH object = {{1, 9, 126, 49}, &data, &control};

    virtual void KeyEnterPush();
};

#endif
