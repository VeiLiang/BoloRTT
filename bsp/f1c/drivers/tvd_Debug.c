// /*
//  * tvd_Debug.c
//  *
//  *  Created on: 2018年12月9日
//  *      Author: VeiLiang
//  */
// //----- (E1601410) --------------------------------------------------------
// int  sub_E1601410(int result)
// {
//   if ( result )
//   {
//     if ( result == 1 )
//     {
//       result = *(_DWORD *)(dword_E1602E64 + 24);
//       __asm("SVCEQ           0x110A");
//     }
//   }
//   else
//   {
//     result = *(_DWORD *)(dword_E1602E64 + 24);
//     __asm (" SVC             0x110A ");
//   }
//   return result;
// }

// //----- (E16002D8) --------------------------------------------------------
// int sub_E16002D8()
// {
//   sub_E1601410(0);
//   return 0;
// }

// //----- (E16002EC) --------------------------------------------------------
// int sub_E16002EC()
// {
//   return 0;
// }

// //----- (E1600328) --------------------------------------------------------
// signed int sub_E1600328()
// {
//   signed int result; // r0@1

//   result = 0x4000000;
//   vF1C0B088 = 0x4000000;
//   return result;
// }

// //----- (E1600338) --------------------------------------------------------
// unsigned int  sub_E1600338(int a1, int a2)
// {
//   unsigned int result; // r0@1

//   result = vF1C0B08C & 0xF800FFFF | (a2 << 16);
//   vF1C0B08C = vF1C0B08C & 0xF800FFFF | (a2 << 16);
//   return result;
// }

// //----- (E1600354) --------------------------------------------------------
// int  sub_E1600354(int a1, signed int a2)
// {
//   int result; // r0@3

//   if ( (unsigned int)a2 > 0x2D0 )
//     a2 = 720;
//   result = a2 | (vF1C0B08C >> 12 << 12);
//   vF1C0B08C = a2 | (vF1C0B08C >> 12 << 12);
//   return result;
// }

// //----- (E1600378) --------------------------------------------------------
// signed int  sub_E1600378(int a1, int a2)
// {
//   int v2; // r7@1
//   signed int result; // r0@1
//   int v8; // r1@4

//   v2 = a1;
//   vF1C0BE04 = 0x8002AAA8;
//   __asm ( "SVC             0x10E" );
//   vF1C0B000 = 0x1CE4;
//   vF1C0BF70 = 0x2010;
//   vF1C0BF0C = 0x10;
//   vF1C0B00C = 0x202080;
//   result = 0xFFFA0A;
//   vF1C0BF6C = 0xFFFA0A;
//   vF1C0BF44 = 0x50823925;
//   vF1C0BF24 = 0x682810A;
//   vF1C0BF28 = 0x6440;
//   vF1C0B014 = 0x20000000;
//   vF1C0BF4C = 0xE70106C;
//   vF1C0B010 = 0x2310080;
//   if ( !v2 )
//   {
//     if ( a2 )
//     {
//       if ( a2 != 1 )
//       {
// LABEL_6:
//         vF1C0BE2C = 0x110000;
//         vF1C0B000 = 0x1F2F;
//         vF1C0BF20 = 0x2C0607;
//         result = 0x10026;
//         vF1C0BE28 = 0x10026;
//         vF1C0BFE0 = 0;
//         return result;
//       }
//       vF1C0B008 = 0x‭1101001‬;
//       vF1C0BF08 = 0x590100;
//       vF1C0BF0C = 0x10;
//       vF1C0BF10 = 0x8A32EC;
//       vF1C0BF14 = 0x‭800000A0;
//       vF1C0B00C = 0x2080;
//       vF1C0BF1C = 0xDC0000;
//       vF1C0BF24 = 0x‭682810A‬;
//       vF1C0BF28 = 0x‭6440‬;
//       vF1C0B014 = 0x20000000;
//       vF1C0BF44 = 0x‭4632‬;
//       vF1C0BF4C = 0x‭E70106C‬;
//       vF1C0BF54 = 0;
//       vF1C0BF58 = 0x82;
//       vF1C0BF6C = 0x‭FFFA0A‬;
//       vF1C0BF80 = 0x‭500000‬;
//       vF1C0BF84 = 0x‭C10000‬;
//       vF1C0B01C = 0x‭870026‬;
//       vF1C0B018 = 0x‭2A098ACB‬;
//       vF1C0BF2C = 0xD74;
//       vF1C0BF74 = 0x343;
//       vF1C0BE04 = 0x8002AAA8;
//       vF1C0B040 = 0x4000310;
//       vF1C0B000 = 0x11;
//       sub_E1600354(0, 720);
//       v8 = 288;
//     }
//     else
//     {
//       vF1C0B008 = 0x10000;
//       vF1C0BF08 = 0x‭590100‬;
//       vF1C0BF10 = ‭0x8A32DD‬;
//       vF1C0BF14 = 0x‭800000A0‬;
//       vF1C0B00C = 0x2080;
//       vF1C0BF1C = 0x‭8A0000‬;
//       vF1C0BF80 = 0x‭500000‬;
//       vF1C0BF84 = 0x‭610000‬;
//       vF1C0B01C = ‭0x820022‬;
//       vF1C0B018 = 0x‭21F07C1F‬;
//       vF1C0BF2C = 0x‭CB74‬;
//       vF1C0BF74 = 0x3C3;
//       vF1C0BE04 = 0x‭8002AAA8‬;
//       vF1C0B040 = 0x‭4000310‬;
//       vF1C0B000 = 0x11;
//       sub_E1600354(0, 720);
//       v8 = 240;
//     }
//     sub_E1600338(0, v8);
//     goto LABEL_6;
//   }
//   return result;
// }

// //----- (E160057C) --------------------------------------------------------
// int  sub_E160057C(int a1, int a2)
// {
//   int result; // r0@2

//   if ( !a2 )
//   {
//     result = vF1C0B09C | 0x1000000;
//     vF1C0B09C |= 0x1000000u;
//   }
//   return result;
// }

// //----- (E1600594) --------------------------------------------------------
// int  sub_E1600594(int result, int a2)
// {
//   if ( !a2 )
//   {
//     result = vF1C0B09C & ~(1 << (result + 24));
//     vF1C0B09C = result;
//   }
//   return result;
// }

// //----- (E16005DC) --------------------------------------------------------
// signed int  sub_E16005DC(signed int result, int a2)
// {
//   if ( !a2 )
//   {
//     result = 1 << (result + 24);
//     vF1C0B094 = result;
//   }
//   return result;
// }

// //----- (E16005F8) --------------------------------------------------------
// int sub_E16005F8()
// {
//   int result; // r0@1

//   result = vF1C0B088 | 1;
//   vF1C0B088 |= 1u;
//   return result;
// }

// //----- (E160060C) --------------------------------------------------------
// unsigned int sub_E160060C()
// {
//   unsigned int result; // r0@1

//   result = vF1C0B088 & 0xFFFFFFFE;
//   vF1C0B088 &= 0xFFFFFFFE;
//   return result;
// }

// //----- (E1600620) --------------------------------------------------------
// signed int  sub_E1600620(int a1, int a2)
// {
//   signed int result; // r0@1

//   result = 0xF1C0B000;
//   vF1C0B080 = a2;
//   vF1C0B088 |= 0x10000000u;
//   return result;
// }

// //----- (E1600638) --------------------------------------------------------
// signed int  sub_E1600638(int a1, int a2)
// {
//   signed int result; // r0@1

//   result = 0xF1C0B000;
//   vF1C0B084 = a2;
//   vF1C0B088 |= 0x10000000u;
//   return result;
// }

// //----- (E1600650) --------------------------------------------------------
// unsigned int  sub_E1600650(int a1, int a2)
// {
//   unsigned int result; // r0@1
//   unsigned int v3; // r0@4

//   result = vF1C0B088;
//   if ( a2 )
//   {
//     if ( a2 == 1 )
//     {
//       v3 = vF1C0B088 & 0xFEFFFFFF;
//     }
//     else
//     {
//       if ( a2 != 2 )
//         goto LABEL_8;
//       v3 = vF1C0B088 | 0x1000000;
//     }
//     result = v3 & 0xFFFFFFEF;
//     goto LABEL_8;
//   }
//   result = vF1C0B088 & 0xFEFFFFFF | 0x10;
// LABEL_8:
//   vF1C0B088 = result;
//   return result;
// }

// //----- (E160068C) --------------------------------------------------------
// int sub_E160068C()
// {
//   signed int v0; // r1@1
//   int v1; // r0@1
//   signed int v2; // r2@1

//   v0 = 0;
//   v1 = vF1C0BE40;
//   v2 = 0;
//   if ( !(vF1C0BE40 & 1) )
//     v0 = 1;
//   if ( v1 & 0x40000 )
//     v2 = 1;
//   return v0 + 16 * v2;
// }

// //----- (E16006B4) --------------------------------------------------------
// unsigned int  sub_E16006B4(int a1, int a2)//(1,4096)
// {
//   unsigned int result; // r0@2

//   if ( a1 )
//   {
//     vF1C0B04C = a2;
//     vF1C0B050 = a2 + 0x200000;
//     vF1C0B054 = 0x200000;
//     vF1C0B048 = ‭0x4040001‬;
//     vF1C0B008 &= 0xFFFFFFFE;
//     result = vF1C0B000 | 0x10;
//   }
//   else
//   {
//     vF1C0B008 |= 1u;
//     result = vF1C0B000 & 0xFFFFFFEF;
//   }
//   vF1C0B000 = result;
//   return result;
// }

// //----- (E160070C) --------------------------------------------------------
// unsigned int  sub_E160070C(int a1)
// {
//   unsigned int result; // r0@2

//   if ( a1 )
//     result = vF1C0BE04 | 1;
//   else
//     result = vF1C0BE04 & 0xFFFFFFFE;
//   vF1C0BE04 = result;
//   return result;
// }
