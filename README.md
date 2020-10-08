# Y-Core

[![sampctl](https://img.shields.io/badge/sampctl-Y--Core-2f2f2f.svg?style=for-the-badge)](https://github.com/aktah/Y-Core)

## Installation

```bash
git clone https://github.com/aktah/Y-Core
```

```pawn
sampctl package ensure
```

เปลี่ยนชื่อ config.inc.template เป็น config.inc และตั้งค่า

## Usage

วิธีสร้างมาโคร OO_TYPE

```pawn
#define Category. OO_TYPE(CATEGORY,Category)

enum E_CATEGORY_SUBCATEGORY {
 E_CATEGORY_Subcategory_id,
 E_CATEGORY_Subcategory_property
}

new CategorySubcategory[MAX_PLAYERS][OPTIONAL_DIMENSION][E_PLAYER_CATEGORY];

// การใช้
new foo = Category.Subcategory[playerid][SOME_INDEX][@property];
```

## Special Thanks

เกมโหมดมาจาก <https://github.com/Y-samp/Y-Core> (Misiur) และมันไม่เคยเสร็จสักทีเลยมีโปรเจคนี้ขึ้นมาเติมความต้องการของผมเอง
เป็นโปรเจคที่ใช้ YSI Library เป็นตัวหลักในการขับเคลื่อนเกมโหมดนี้ โปรเจคนี้จะไม่เกิดขึ้นถ้าหากไม่มีพวกเขา Y Less, Misiur, Zeex, BlueG, maddinat0r, Incognito และคนอื่น ๆ อีกมากมาย
