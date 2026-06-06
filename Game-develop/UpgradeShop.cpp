//實作商店

#include "UpgradeShop.h"
#include <string>

UpgradeShop::UpgradeShop() {
    // 設定升級面板按鈕的位置
    upgradeHpBtn = { 250, 220, 300, 50 };
    upgradeHungerBtn = { 250, 320, 300, 40 };
    upgradeMoodBtn = { 250, 420, 300, 40 };
    closeShopBtn = { 300, 500, 200, 40 };
}

bool UpgradeShop::Update(Vector2 mousePos, PetStats* stats) {
    // 升級生命
    if (CheckCollisionPointRec(mousePos, upgradeHpBtn)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyUpgradeMaxHealth(100, 20.0f);
        }
    }

    // 升級飽食上限 (消耗 $100，提升 20 點上限)
    if (CheckCollisionPointRec(mousePos, upgradeHungerBtn)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyUpgradeMaxHunger(100, 20.0f);
        }
    }

    // 升級心情上限 (消耗 $100，提升 20 點上限)
    if (CheckCollisionPointRec(mousePos, upgradeMoodBtn)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyUpgradeMaxMood(100, 20.0f);
        }
    }

    // 返回重開局
    if (CheckCollisionPointRec(mousePos, closeShopBtn)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true; 
        }
    }
    return false;
}

void UpgradeShop::Draw(Font font, PetStats* stats, int screenWidth, int screenHeight) {
    // 全螢幕深藍色科技感背景
    DrawRectangle(0, 0, screenWidth, screenHeight, DARKBLUE);

    // 繪製中央裝飾面板
    DrawRectangle(100, 50, screenWidth - 200, screenHeight - 100, Fade(BLACK, 0.4f));
    DrawRectangleLines(100, 50, screenWidth - 200, screenHeight - 100, GOLD);

    DrawTextEx(font, u8"大雞雞基因研究院 (永久升級)", Vector2{ 200, 80 }, 32, 2, GOLD);

    // 顯示玩家當前的金幣與生命上限資產
    std::string assetCoin = u8"擁有研究金幣: $" + std::to_string(stats->GetCoins());
    std::string statusText = u8"生命上限: " + std::to_string((int)stats->GetMaxHealth()) + 
                             u8"  |  飽食上限: " + std::to_string((int)stats->GetMaxHunger()) + 
                             u8"  |  心情上限: " + std::to_string((int)stats->GetMaxMood());

    DrawTextEx(font, assetCoin.c_str(), Vector2{ 150, 140 }, 24, 2, WHITE);
    DrawTextEx(font, statusText.c_str(), Vector2{ 150, 170 }, 20, 2, LIGHTGRAY);

    // 按鈕 1：生命升級
    DrawRectangleRec(upgradeHpBtn, MAROON);
    DrawRectangleLinesEx(upgradeHpBtn, 2, GOLD);
    DrawTextEx(font, u8"永久改造生命上限 (+20) $100", Vector2{ upgradeHpBtn.x + 15, upgradeHpBtn.y + 10 }, 18, 2, WHITE);

    // 按鈕 2：⚠️ 新增：飽食升級
    DrawRectangleRec(upgradeHungerBtn, ORANGE);
    DrawRectangleLinesEx(upgradeHungerBtn, 2, GOLD);
    DrawTextEx(font, u8"永久改造飽食上限 (+20) $100", Vector2{ upgradeHungerBtn.x + 15, upgradeHungerBtn.y + 10 }, 18, 2, WHITE);

    // 按鈕 3：⚠️ 新增：心情升級
    DrawRectangleRec(upgradeMoodBtn, DARKPURPLE);
    DrawRectangleLinesEx(upgradeMoodBtn, 2, GOLD);
    DrawTextEx(font, u8"永久改造心情上限 (+20) $100", Vector2{ upgradeMoodBtn.x + 15, upgradeMoodBtn.y + 10 }, 18, 2, WHITE);

    // 返回按鈕
    DrawRectangleRec(closeShopBtn, DARKGRAY);
    DrawRectangleLinesEx(closeShopBtn, 2, WHITE);
    DrawTextEx(font, u8"返回並重開局", Vector2{ closeShopBtn.x + 15, closeShopBtn.y + 8 }, 20, 2, WHITE);
}

UpgradeShop::~UpgradeShop() {
}