/* vim:ts=4
 *
 * Copyleft 2012…2014	Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__MCU__ATXMEGA128_A1U_PORTS_H__INCLUDED
#define MULABS_AVR__MCU__ATXMEGA128_A1U_PORTS_H__INCLUDED

namespace mulabs {
namespace avr {

static constexpr size_t kPORTA_DIR		= 0x0600;
static constexpr size_t kPORTA_DIRSET	= 0x0601;
static constexpr size_t kPORTA_DIRCLR	= 0x0602;
static constexpr size_t kPORTA_DIRTGL	= 0x0603;
static constexpr size_t kPORTA_OUT		= 0x0604;
static constexpr size_t kPORTA_OUTSET	= 0x0605;
static constexpr size_t kPORTA_OUTCLR	= 0x0606;
static constexpr size_t kPORTA_OUTTGL	= 0x0607;
static constexpr size_t kPORTA_IN		= 0x0608;
static constexpr size_t kPORTA_INTCTRL	= 0x0609;
static constexpr size_t kPORTA_INT0MASK	= 0x060A;
static constexpr size_t kPORTA_INT1MASK	= 0x060B;
static constexpr size_t kPORTA_INTFLAGS	= 0x060C;
static constexpr size_t kPORTA_PIN0CTRL	= 0x0610;
static constexpr size_t kPORTA_PIN1CTRL	= 0x0611;
static constexpr size_t kPORTA_PIN2CTRL	= 0x0612;
static constexpr size_t kPORTA_PIN3CTRL	= 0x0613;
static constexpr size_t kPORTA_PIN4CTRL	= 0x0614;
static constexpr size_t kPORTA_PIN5CTRL	= 0x0615;
static constexpr size_t kPORTA_PIN6CTRL	= 0x0616;
static constexpr size_t kPORTA_PIN7CTRL	= 0x0617;

static constexpr size_t kPORTB_DIR		= 0x0620;
static constexpr size_t kPORTB_DIRSET	= 0x0621;
static constexpr size_t kPORTB_DIRCLR	= 0x0622;
static constexpr size_t kPORTB_DIRTGL	= 0x0623;
static constexpr size_t kPORTB_OUT		= 0x0624;
static constexpr size_t kPORTB_OUTSET	= 0x0625;
static constexpr size_t kPORTB_OUTCLR	= 0x0626;
static constexpr size_t kPORTB_OUTTGL	= 0x0627;
static constexpr size_t kPORTB_IN		= 0x0628;
static constexpr size_t kPORTB_INTCTRL	= 0x0629;
static constexpr size_t kPORTB_INT0MASK	= 0x062A;
static constexpr size_t kPORTB_INT1MASK	= 0x062B;
static constexpr size_t kPORTB_INTFLAGS	= 0x062C;
static constexpr size_t kPORTB_PIN0CTRL	= 0x0630;
static constexpr size_t kPORTB_PIN1CTRL	= 0x0631;
static constexpr size_t kPORTB_PIN2CTRL	= 0x0632;
static constexpr size_t kPORTB_PIN3CTRL	= 0x0633;
static constexpr size_t kPORTB_PIN4CTRL	= 0x0634;
static constexpr size_t kPORTB_PIN5CTRL	= 0x0635;
static constexpr size_t kPORTB_PIN6CTRL	= 0x0636;
static constexpr size_t kPORTB_PIN7CTRL	= 0x0637;

static constexpr size_t kPORTC_DIR		= 0x0640;
static constexpr size_t kPORTC_DIRSET	= 0x0641;
static constexpr size_t kPORTC_DIRCLR	= 0x0642;
static constexpr size_t kPORTC_DIRTGL	= 0x0643;
static constexpr size_t kPORTC_OUT		= 0x0644;
static constexpr size_t kPORTC_OUTSET	= 0x0645;
static constexpr size_t kPORTC_OUTCLR	= 0x0646;
static constexpr size_t kPORTC_OUTTGL	= 0x0647;
static constexpr size_t kPORTC_IN		= 0x0648;
static constexpr size_t kPORTC_INTCTRL	= 0x0649;
static constexpr size_t kPORTC_INT0MASK	= 0x064A;
static constexpr size_t kPORTC_INT1MASK	= 0x064B;
static constexpr size_t kPORTC_INTFLAGS	= 0x064C;
static constexpr size_t kPORTC_PIN0CTRL	= 0x0650;
static constexpr size_t kPORTC_PIN1CTRL	= 0x0651;
static constexpr size_t kPORTC_PIN2CTRL	= 0x0652;
static constexpr size_t kPORTC_PIN3CTRL	= 0x0653;
static constexpr size_t kPORTC_PIN4CTRL	= 0x0654;
static constexpr size_t kPORTC_PIN5CTRL	= 0x0655;
static constexpr size_t kPORTC_PIN6CTRL	= 0x0656;
static constexpr size_t kPORTC_PIN7CTRL	= 0x0657;

static constexpr size_t kPORTD_DIR		= 0x0660;
static constexpr size_t kPORTD_DIRSET	= 0x0661;
static constexpr size_t kPORTD_DIRCLR	= 0x0662;
static constexpr size_t kPORTD_DIRTGL	= 0x0663;
static constexpr size_t kPORTD_OUT		= 0x0664;
static constexpr size_t kPORTD_OUTSET	= 0x0665;
static constexpr size_t kPORTD_OUTCLR	= 0x0666;
static constexpr size_t kPORTD_OUTTGL	= 0x0667;
static constexpr size_t kPORTD_IN		= 0x0668;
static constexpr size_t kPORTD_INTCTRL	= 0x0669;
static constexpr size_t kPORTD_INT0MASK	= 0x066A;
static constexpr size_t kPORTD_INT1MASK	= 0x066B;
static constexpr size_t kPORTD_INTFLAGS	= 0x066C;
static constexpr size_t kPORTD_PIN0CTRL	= 0x0670;
static constexpr size_t kPORTD_PIN1CTRL	= 0x0671;
static constexpr size_t kPORTD_PIN2CTRL	= 0x0672;
static constexpr size_t kPORTD_PIN3CTRL	= 0x0673;
static constexpr size_t kPORTD_PIN4CTRL	= 0x0674;
static constexpr size_t kPORTD_PIN5CTRL	= 0x0675;
static constexpr size_t kPORTD_PIN6CTRL	= 0x0676;
static constexpr size_t kPORTD_PIN7CTRL	= 0x0677;

static constexpr size_t kPORTE_DIR		= 0x0680;
static constexpr size_t kPORTE_DIRSET	= 0x0681;
static constexpr size_t kPORTE_DIRCLR	= 0x0682;
static constexpr size_t kPORTE_DIRTGL	= 0x0683;
static constexpr size_t kPORTE_OUT		= 0x0684;
static constexpr size_t kPORTE_OUTSET	= 0x0685;
static constexpr size_t kPORTE_OUTCLR	= 0x0686;
static constexpr size_t kPORTE_OUTTGL	= 0x0687;
static constexpr size_t kPORTE_IN		= 0x0688;
static constexpr size_t kPORTE_INTCTRL	= 0x0689;
static constexpr size_t kPORTE_INT0MASK	= 0x068A;
static constexpr size_t kPORTE_INT1MASK	= 0x068B;
static constexpr size_t kPORTE_INTFLAGS	= 0x068C;
static constexpr size_t kPORTE_PIN0CTRL	= 0x0690;
static constexpr size_t kPORTE_PIN1CTRL	= 0x0691;
static constexpr size_t kPORTE_PIN2CTRL	= 0x0692;
static constexpr size_t kPORTE_PIN3CTRL	= 0x0693;
static constexpr size_t kPORTE_PIN4CTRL	= 0x0694;
static constexpr size_t kPORTE_PIN5CTRL	= 0x0695;
static constexpr size_t kPORTE_PIN6CTRL	= 0x0696;
static constexpr size_t kPORTE_PIN7CTRL	= 0x0697;

static constexpr size_t kPORTF_DIR		= 0x06A0;
static constexpr size_t kPORTF_DIRSET	= 0x06A1;
static constexpr size_t kPORTF_DIRCLR	= 0x06A2;
static constexpr size_t kPORTF_DIRTGL	= 0x06A3;
static constexpr size_t kPORTF_OUT		= 0x06A4;
static constexpr size_t kPORTF_OUTSET	= 0x06A5;
static constexpr size_t kPORTF_OUTCLR	= 0x06A6;
static constexpr size_t kPORTF_OUTTGL	= 0x06A7;
static constexpr size_t kPORTF_IN		= 0x06A8;
static constexpr size_t kPORTF_INTCTRL	= 0x06A9;
static constexpr size_t kPORTF_INT0MASK	= 0x06AA;
static constexpr size_t kPORTF_INT1MASK	= 0x06AB;
static constexpr size_t kPORTF_INTFLAGS	= 0x06AC;
static constexpr size_t kPORTF_PIN0CTRL	= 0x06B0;
static constexpr size_t kPORTF_PIN1CTRL	= 0x06B1;
static constexpr size_t kPORTF_PIN2CTRL	= 0x06B2;
static constexpr size_t kPORTF_PIN3CTRL	= 0x06B3;
static constexpr size_t kPORTF_PIN4CTRL	= 0x06B4;
static constexpr size_t kPORTF_PIN5CTRL	= 0x06B5;
static constexpr size_t kPORTF_PIN6CTRL	= 0x06B6;
static constexpr size_t kPORTF_PIN7CTRL	= 0x06B7;

static constexpr size_t kPORTH_DIR		= 0x06E0;
static constexpr size_t kPORTH_DIRSET	= 0x06E1;
static constexpr size_t kPORTH_DIRCLR	= 0x06E2;
static constexpr size_t kPORTH_DIRTGL	= 0x06E3;
static constexpr size_t kPORTH_OUT		= 0x06E4;
static constexpr size_t kPORTH_OUTSET	= 0x06E5;
static constexpr size_t kPORTH_OUTCLR	= 0x06E6;
static constexpr size_t kPORTH_OUTTGL	= 0x06E7;
static constexpr size_t kPORTH_IN		= 0x06E8;
static constexpr size_t kPORTH_INTCTRL	= 0x06E9;
static constexpr size_t kPORTH_INT0MASK	= 0x06EA;
static constexpr size_t kPORTH_INT1MASK	= 0x06EB;
static constexpr size_t kPORTH_INTFLAGS	= 0x06EC;
static constexpr size_t kPORTH_PIN0CTRL	= 0x06F0;
static constexpr size_t kPORTH_PIN1CTRL	= 0x06F1;
static constexpr size_t kPORTH_PIN2CTRL	= 0x06F2;
static constexpr size_t kPORTH_PIN3CTRL	= 0x06F3;
static constexpr size_t kPORTH_PIN4CTRL	= 0x06F4;
static constexpr size_t kPORTH_PIN5CTRL	= 0x06F5;
static constexpr size_t kPORTH_PIN6CTRL	= 0x06F6;
static constexpr size_t kPORTH_PIN7CTRL	= 0x06F7;

static constexpr size_t kPORTJ_DIR		= 0x0700;
static constexpr size_t kPORTJ_DIRSET	= 0x0701;
static constexpr size_t kPORTJ_DIRCLR	= 0x0702;
static constexpr size_t kPORTJ_DIRTGL	= 0x0703;
static constexpr size_t kPORTJ_OUT		= 0x0704;
static constexpr size_t kPORTJ_OUTSET	= 0x0705;
static constexpr size_t kPORTJ_OUTCLR	= 0x0706;
static constexpr size_t kPORTJ_OUTTGL	= 0x0707;
static constexpr size_t kPORTJ_IN		= 0x0708;
static constexpr size_t kPORTJ_INTCTRL	= 0x0709;
static constexpr size_t kPORTJ_INT0MASK	= 0x070A;
static constexpr size_t kPORTJ_INT1MASK	= 0x070B;
static constexpr size_t kPORTJ_INTFLAGS	= 0x070C;
static constexpr size_t kPORTJ_PIN0CTRL	= 0x0710;
static constexpr size_t kPORTJ_PIN1CTRL	= 0x0711;
static constexpr size_t kPORTJ_PIN2CTRL	= 0x0712;
static constexpr size_t kPORTJ_PIN3CTRL	= 0x0713;
static constexpr size_t kPORTJ_PIN4CTRL	= 0x0714;
static constexpr size_t kPORTJ_PIN5CTRL	= 0x0715;
static constexpr size_t kPORTJ_PIN6CTRL	= 0x0716;
static constexpr size_t kPORTJ_PIN7CTRL	= 0x0717;

static constexpr size_t kPORTK_DIR		= 0x0720;
static constexpr size_t kPORTK_DIRSET	= 0x0721;
static constexpr size_t kPORTK_DIRCLR	= 0x0722;
static constexpr size_t kPORTK_DIRTGL	= 0x0723;
static constexpr size_t kPORTK_OUT		= 0x0724;
static constexpr size_t kPORTK_OUTSET	= 0x0725;
static constexpr size_t kPORTK_OUTCLR	= 0x0726;
static constexpr size_t kPORTK_OUTTGL	= 0x0727;
static constexpr size_t kPORTK_IN		= 0x0728;
static constexpr size_t kPORTK_INTCTRL	= 0x0729;
static constexpr size_t kPORTK_INT0MASK	= 0x072A;
static constexpr size_t kPORTK_INT1MASK	= 0x072B;
static constexpr size_t kPORTK_INTFLAGS	= 0x072C;
static constexpr size_t kPORTK_PIN0CTRL	= 0x0730;
static constexpr size_t kPORTK_PIN1CTRL	= 0x0731;
static constexpr size_t kPORTK_PIN2CTRL	= 0x0732;
static constexpr size_t kPORTK_PIN3CTRL	= 0x0733;
static constexpr size_t kPORTK_PIN4CTRL	= 0x0734;
static constexpr size_t kPORTK_PIN5CTRL	= 0x0735;
static constexpr size_t kPORTK_PIN6CTRL	= 0x0736;
static constexpr size_t kPORTK_PIN7CTRL	= 0x0737;

static constexpr size_t kPORTQ_DIR		= 0x07C0;
static constexpr size_t kPORTQ_DIRSET	= 0x07C1;
static constexpr size_t kPORTQ_DIRCLR	= 0x07C2;
static constexpr size_t kPORTQ_DIRTGL	= 0x07C3;
static constexpr size_t kPORTQ_OUT		= 0x07C4;
static constexpr size_t kPORTQ_OUTSET	= 0x07C5;
static constexpr size_t kPORTQ_OUTCLR	= 0x07C6;
static constexpr size_t kPORTQ_OUTTGL	= 0x07C7;
static constexpr size_t kPORTQ_IN		= 0x07C8;
static constexpr size_t kPORTQ_INTCTRL	= 0x07C9;
static constexpr size_t kPORTQ_INT0MASK	= 0x07CA;
static constexpr size_t kPORTQ_INT1MASK	= 0x07CB;
static constexpr size_t kPORTQ_INTFLAGS	= 0x07CC;
static constexpr size_t kPORTQ_PIN0CTRL	= 0x07D0;
static constexpr size_t kPORTQ_PIN1CTRL	= 0x07D1;
static constexpr size_t kPORTQ_PIN2CTRL	= 0x07D2;
static constexpr size_t kPORTQ_PIN3CTRL	= 0x07D3;
static constexpr size_t kPORTQ_PIN4CTRL	= 0x07D4;
static constexpr size_t kPORTQ_PIN5CTRL	= 0x07D5;
static constexpr size_t kPORTQ_PIN6CTRL	= 0x07D6;
static constexpr size_t kPORTQ_PIN7CTRL	= 0x07D7;

static constexpr size_t kPORTR_DIR		= 0x07E0;
static constexpr size_t kPORTR_DIRSET	= 0x07E1;
static constexpr size_t kPORTR_DIRCLR	= 0x07E2;
static constexpr size_t kPORTR_DIRTGL	= 0x07E3;
static constexpr size_t kPORTR_OUT		= 0x07E4;
static constexpr size_t kPORTR_OUTSET	= 0x07E5;
static constexpr size_t kPORTR_OUTCLR	= 0x07E6;
static constexpr size_t kPORTR_OUTTGL	= 0x07E7;
static constexpr size_t kPORTR_IN		= 0x07E8;
static constexpr size_t kPORTR_INTCTRL	= 0x07E9;
static constexpr size_t kPORTR_INT0MASK	= 0x07EA;
static constexpr size_t kPORTR_INT1MASK	= 0x07EB;
static constexpr size_t kPORTR_INTFLAGS	= 0x07EC;
static constexpr size_t kPORTR_PIN0CTRL	= 0x07F0;
static constexpr size_t kPORTR_PIN1CTRL	= 0x07F1;
static constexpr size_t kPORTR_PIN2CTRL	= 0x07F2;
static constexpr size_t kPORTR_PIN3CTRL	= 0x07F3;
static constexpr size_t kPORTR_PIN4CTRL	= 0x07F4;
static constexpr size_t kPORTR_PIN5CTRL	= 0x07F5;
static constexpr size_t kPORTR_PIN6CTRL	= 0x07F6;
static constexpr size_t kPORTR_PIN7CTRL	= 0x07F7;

} // namespace avr
} // namespace mulabs

#endif

