#include "../../PVZ.h"

namespace PVZ {
    class PVZApp;
    class Board;
    class GameButton : PVZ::BaseClass
    {
    public:
        GameButton(int address) : PVZ::BaseClass(address) {};
        GameButton() : PVZ::BaseClass(INVALID_BASEADDRESS) {};
        /// @brief Button在屏幕上的X坐标
        INT_PROPERTY(mX, __get_mX, __set_mX, 0x8);
        /// @brief Button在屏幕上的Y坐标
        INT_PROPERTY(mY, __get_mY, __set_mY, 0xC);
        /// @brief Button在屏幕上的宽
        INT_PROPERTY(mWidth, __get_mWidth, __set_mWidth, 0x10);
        /// @brief Button在屏幕上的高
        INT_PROPERTY(mHeight, __get_mHeight, __set_mHeight, 0x14);
        /// @brief Button是否被禁用
        T_PROPERTY(BOOL, mDisabled, __get_Disabled, __set_Disabled, 0x1A);
        /// @brief Button的ID
        INT_PROPERTY(mId, __get_mId, __set_mId, 0x7C);
        /// @brief 获取Button所在的PVZApp
        PVZApp GetPVZApp();
        /// @brief 获取Button的父控件
        Widget GetParentWidget();
        /// @brief 获取Button的文字
        PVZString GetLabel();
    };
}