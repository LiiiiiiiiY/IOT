/**
  ******************************************************************************
  * @file    stm32l0xx_ll_cortex.h
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    31-May-2016
  * @brief   Header file of CORTEX LL module.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The LL CORTEX driver contains a set of generic APIs that can be
    used by user:
      (+) SYSTICK configuration used by @ref LL_mDelay and @ref LL_Init1msTick
          functions
      (+) Low power mode configuration (SCB register of Cortex-MCU)
      (+) MPU API to configure and enable regions
      (+) API to access to MCU info (CPUID register)

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_LL_CORTEX_H
#define __STM32L0xx_LL_CORTEX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"

/** @addtogroup STM32L0xx_LL_Driver
  * @{
  */

/** @defgroup CORTEX_LL CORTEX
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup CORTEX_LL_Exported_Constants CORTEX Exported Constants
  * @{
  */

/** @defgroup CORTEX_LL_EC_CLKSOURCE_HCLK SYSTICK Clock Source
  * @{
  */
#define LL_SYSTICK_CLKSOURCE_HCLK_DIV8     ((uint32_t)0x00000000U)                 /*!< AHB clock divided by 8 selected as SysTick clock source.*/
#define LL_SYSTICK_CLKSOURCE_HCLK          ((uint32_t)SysTick_CTRL_CLKSOURCE_Msk) /*!< AHB clock selected as SysTick clock source. */
/**
  * @}
  */

#if __MPU_PRESENT

/** @defgroup CORTEX_LL_EC_CTRL_HFNMI_PRIVDEF MPU Control
  * @{
  */
#define LL_MPU_CTRL_HFNMI_PRIVDEF_NONE     ((uint32_t)0x00000000U)                            /*!< Disable NMI and privileged SW access */
#define LL_MPU_CTRL_HARDFAULT_NMI          MPU_CTRL_HFNMIENA_Msk                             /*!< Enables the operation of MPU during hard fault, NMI, and FAULTMASK handlers */
#define LL_MPU_CTRL_PRIVILEGED_DEFAULT     MPU_CTRL_PRIVDEFENA_Msk                           /*!< Enable privileged software access to default memory map */
#define LL_MPU_CTRL_HFNMI_PRIVDEF          (MPU_CTRL_HFNMIENA_Msk | MPU_CTRL_PRIVDEFENA_Msk) /*!< Enable NMI and privileged SW access */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_REGION MPU Region Number
  * @{
  */
#define LL_MPU_REGION_NUMBER0              ((uint32_t)0x00U) /*!< REGION Number 0 */
#define LL_MPU_REGION_NUMBER1              ((uint32_t)0x01U) /*!< REGION Number 1 */
#define LL_MPU_REGION_NUMBER2              ((uint32_t)0x02U) /*!< REGION Number 2 */
#define LL_MPU_REGION_NUMBER3              ((uint32_t)0x03U) /*!< REGION Number 3 */
#define LL_MPU_REGION_NUMBER4              ((uint32_t)0x04U) /*!< REGION Number 4 */
#define LL_MPU_REGION_NUMBER5              ((uint32_t)0x05U) /*!< REGION Number 5 */
#define LL_MPU_REGION_NUMBER6              ((uint32_t)0x06U) /*!< REGION Number 6 */
#define LL_MPU_REGION_NUMBER7              ((uint32_t)0x07U) /*!< REGION Number 7 */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_REGION_SIZE MPU Region Size
  * @{
  */
#define LL_MPU_REGION_SIZE_32B             ((uint32_t)(0x04U << MPU_RASR_SIZE_Pos)) /*!< 32B Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_64B             ((uint32_t)(0x05U << MPU_RASR_SIZE_Pos)) /*!< 64B Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_128B            ((uint32_t)(0x06U << MPU_RASR_SIZE_Pos)) /*!< 128B Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_256B            ((uint32_t)(0x07U << MPU_RASR_SIZE_Pos)) /*!< 256B Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_512B            ((uint32_t)(0x08U << MPU_RASR_SIZE_Pos)) /*!< 512B Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_1KB             ((uint32_t)(0x09U << MPU_RASR_SIZE_Pos)) /*!< 1KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_2KB             ((uint32_t)(0x0AU << MPU_RASR_SIZE_Pos)) /*!< 2KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_4KB             ((uint32_t)(0x0BU << MPU_RASR_SIZE_Pos)) /*!< 4KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_8KB             ((uint32_t)(0x0CU << MPU_RASR_SIZE_Pos)) /*!< 8KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_16KB            ((uint32_t)(0x0DU << MPU_RASR_SIZE_Pos)) /*!< 16KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_32KB            ((uint32_t)(0x0EU << MPU_RASR_SIZE_Pos)) /*!< 32KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_64KB            ((uint32_t)(0x0FU << MPU_RASR_SIZE_Pos)) /*!< 64KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_128KB           ((uint32_t)(0x10U << MPU_RASR_SIZE_Pos)) /*!< 128KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_256KB           ((uint32_t)(0x11U << MPU_RASR_SIZE_Pos)) /*!< 256KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_512KB           ((uint32_t)(0x12U << MPU_RASR_SIZE_Pos)) /*!< 512KB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_1MB             ((uint32_t)(0x13U << MPU_RASR_SIZE_Pos)) /*!< 1MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_2MB             ((uint32_t)(0x14U << MPU_RASR_SIZE_Pos)) /*!< 2MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_4MB             ((uint32_t)(0x15U << MPU_RASR_SIZE_Pos)) /*!< 4MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_8MB             ((uint32_t)(0x16U << MPU_RASR_SIZE_Pos)) /*!< 8MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_16MB            ((uint32_t)(0x17U << MPU_RASR_SIZE_Pos)) /*!< 16MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_32MB            ((uint32_t)(0x18U << MPU_RASR_SIZE_Pos)) /*!< 32MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_64MB            ((uint32_t)(0x19U << MPU_RASR_SIZE_Pos)) /*!< 64MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_128MB           ((uint32_t)(0x1AU << MPU_RASR_SIZE_Pos)) /*!< 128MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_256MB           ((uint32_t)(0x1BU << MPU_RASR_SIZE_Pos)) /*!< 256MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_512MB           ((uint32_t)(0x1CU << MPU_RASR_SIZE_Pos)) /*!< 512MB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_1GB             ((uint32_t)(0x1DU << MPU_RASR_SIZE_Pos)) /*!< 1GB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_2GB             ((uint32_t)(0x1EU << MPU_RASR_SIZE_Pos)) /*!< 2GB Size of the MPU protection region */
#define LL_MPU_REGION_SIZE_4GB             ((uint32_t)(0x1FU << MPU_RASR_SIZE_Pos)) /*!< 4GB Size of the MPU protection region */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_REGION_PRIVILEDGES MPU Region Privileges
  * @{
  */
#define LL_MPU_REGION_NO_ACCESS            ((uint32_t)(0x00U << MPU_RASR_AP_Pos)) /*!< No access*/
#define LL_MPU_REGION_PRIV_RW              ((uint32_t)(0x01U << MPU_RASR_AP_Pos)) /*!< RW privileged (privileged access only)*/
#define LL_MPU_REGION_PRIV_RW_URO          ((uint32_t)(0x02U << MPU_RASR_AP_Pos)) /*!< RW privileged - RO user (Write in a user program generates a fault) */
#define LL_MPU_REGION_FULL_ACCESS          ((uint32_t)(0x03U << MPU_RASR_AP_Pos)) /*!< RW privileged & user (Full access) */
#define LL_MPU_REGION_PRIV_RO              ((uint32_t)(0x05U << MPU_RASR_AP_Pos)) /*!< RO privileged (privileged read only)*/
#define LL_MPU_REGION_PRIV_RO_URO          ((uint32_t)(0x06U << MPU_RASR_AP_Pos)) /*!< RO privileged & user (read only) */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_TEX MPU TEX Level
  * @{
  */
#define LL_MPU_TEX_LEVEL0                  ((uint32_t)(0x00U << MPU_RASR_TEX_Pos)) /*!< b000 for TEX bits */
#define LL_MPU_TEX_LEVEL1                  ((uint32_t)(0x01U << MPU_RASR_TEX_Pos)) /*!< b001 for TEX bits */
#define LL_MPU_TEX_LEVEL2                  ((uint32_t)(0x02U << MPU_RASR_TEX_Pos)) /*!< b010 for TEX bits */
#define LL_MPU_TEX_LEVEL4                  ((uint32_t)(0x04U << MPU_RASR_TEX_Pos)) /*!< b100 for TEX bits */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_INSTRUCTION_ACCESS MPU Instruction Access
  * @{
  */
#define LL_MPU_INSTRUCTION_ACCESS_ENABLE   ((uint32_t)0x00U) /*!< Instruction fetches enabled */
#define LL_MPU_INSTRUCTION_ACCESS_DISABLE  MPU_RASR_XN_Msk  /*!< Instruction fetches disabled*/
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_SHAREABLE_ACCESS MPU Shareable Access
  * @{
  */
#define LL_MPU_ACCESS_SHAREABLE            MPU_RASR_S_Msk   /*!< Shareable memory attribute */
#define LL_MPU_ACCESS_NOT_SHAREABLE        ((uint32_t)0x00U) /*!< Not Shareable memory attribute */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_CACHEABLE_ACCESS MPU Cacheable Access
  * @{
  */
#define LL_MPU_ACCESS_CACHEABLE            MPU_RASR_C_Msk   /*!< Cacheable memory attribute */
#define LL_MPU_ACCESS_NOT_CACHEABLE        ((uint32_t)0x00U) /*!< Not Cacheable memory attribute */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_BUFFERABLE_ACCESS MPU Bufferable Access
  * @{
  */
#define LL_MPU_ACCESS_BUFFERABLE           MPU_RASR_B_Msk   /*!< Bufferable memory attribute */
#define LL_MPU_ACCESS_NOT_BUFFERABLE       ((uint32_t)0x00U) /*!< Not Bufferable memory attribute */
/**
  * @}
  */
#endif /* __MPU_PRESENT */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup CORTEX_LL_Exported_Functions CORTEX Exported Functions
  * @{
  */

/** @defgroup CORTEX_LL_EF_SYSTICK SYSTICK
  * @{
  */

/**
  * @brief  This function checks if the Systick counter flag is active or not.
  * @note   It can be used in timeout function on application side.
  * @rmtoll STK_CTRL     COUNTFLAG     LL_SYSTICK_IsActiveCounterFlag
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

/**
  * @brief  Configures the SysTick clock source
  * @rmtoll STK_CTRL     CLKSOURCE     LL_SYSTICK_SetClkSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK_DIV8
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK
  * @retval None
  */
__STATIC_INLINE void LL_SYSTICK_SetClkSource(uint32_t Source)
{
  if (Source == LL_SYSTICK_CLKSOURCE_HCLK)
  {
    SET_BIT(SysTick->CTRL, LL_SYSTICK_CLKSOURCE_HCLK);
  }
  else
  {
    CLEAR_BIT(SysTick->CTRL, LL_SYSTICK_CLKSOURCE_HCLK);
  }
}

/**
  * @brief  Get the SysTick clock source
  * @rmtoll STK_CTRL     CLKSOURCE     LL_SYSTICK_GetClkSource
  * @retval R< 4TEX_LL,M|0VICES; LOSS O * @retval State of bit (1 or eTRL     COUNTFLAG   bleDnt32_t Source)
{
  if (Source == LL_SYSTICK_CLKE_HCLK)
  {
    SET_privileick->CTRexcepze of tqet tK_CLKSOURCE_HCLK);
  }
  ourcINT   LL_S if (SourceprivilITis parameter can be one of the following values:
  *     privilITTRL     COUNTE_HCLK
  * @retval None of bit (1 oourcINT*/
_CLKE_HCLK)
  {
    SET_LL_EC_CTick->CTRexcepze of tqet tK_CLKSOURCE_HCLK);
  }
  ourcINT   LL_S if (SourceLL_EC_CITis parameter can be one of the following values:
  *     LL_EC_CITTRL     COUNTce(uint32_t Source)
{
 e of bit (1 oourcINT*/
_CLKE_HCLK)
  {
    SET_Corted_Function-------ill SruptunctX_LL_EC_n bS_ENABLE/**
  * @brief  This functiourcINT   LL_S if (SourceIsprivildITis parameter c * @note   It can be used in timeout function on application side.
privildITTRL     COUNTFLAG    bleDnt32_t Source)
{
 e of bit (1 oourcINT*/
_ State of bit (1 oourcINT*/
__CLKE_HCLK)
  {
 k   /*!< Cacheable memory attribute F_LOW_POWER_MODE LOW POWER MODE-------------------*/
/** @defgrouProLE  n bRTEs sleepded << Mlc APIs that c**
  * @brief CB_SCR  LL_SSLEEPDEEP LL_S ifLPM privilSleepis parameter can be one of the following values:
LPM privilSleepTRL     COUNT**ClearSSLEEPDEEP   It   (+) SYons CemE_Msk) /2_t)(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEEPDEEP*/
__STATIC_INLINE uint32_t LLProLE  n bRTEs deepdsleepded << Mlc APIs that c**
  * @brief CB_SCR  LL_SSLEEPDEEP LL_S ifLPM privilDeepSleepis parameter can be one of the following values:
LPM privilDeepSleepTRL     COUNT**   SSLEEPDEEP   It   (+) SYons CemE_Msk) /2_t)(
   e ofNTE_HCLK
 CB->SCR,    ((uint32_CB_SCR_SLEEPDEEP*/
__STATIC_INLINE uint32_t LL_SYSTICK_Issleep-on-ex ItwhenTFLAG  riefutionurceIsprivildITis parameter c * @note   It can be used in timeout fICK_IsslNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEEPDEEP*/
__STATItClkSourceRO prClkS flag is active2_ttPDEEprodu_Issurc1meout fthisbe used in-.]
  in timeout fuegio2_CB_R_SLEEPDEEegioer in the dooooemptccesi in timeout fplication side.T**   SSLEEPDEEPONEXITSYons CemE_Msk) /2_t)OnEt32(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEEPDEEP*/
_OnEt32TIC_INLINE uint32_t LL_SYONEXITS_Issleep-on-ex ItwhenTFLAG  riefutionurceIsprivildITis parameter c * @note   It can beONEXITd in timeout fICK_IsslNTce(uintDonterng valuCB_SCR_SLEEPDEEurceRO prClkS flag isn side.T**   SSLEEPDEEPONEXITSYons CemEnctX_LL/2_t)OnEt32(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEnctX_LL/2_t)OnEt32_STATIC_INLINE uint32_t LLPONEXITS_Issleep-on-ex ItwhenTFLAG  riefutionurceIsp
  * @brief CB_SCR  LL_SSLEEPDEEP LL_S ifLPMONEXITd in timeout fICK_IsslNTce(uint   /*!<  */
this lade.e used ins,e to pdditions /*!< e used ins,eretvwake @be  r in the doooopone of t flag isn side.T**   SSLEEPEVEONPEND   (+) SYons CemEE*/
OnPend(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEEPDEEPE*/
OnPendTIC_INLINE uint32_t LEVEONPEND _Issleep-on-ex ItwhenTFLAG  riefutionurceIsprivildITis parameter c * @note   It caEVONPENDd in timeout fICK_IsslNTce(uintOnlymeout fune used ins
_  */
thretvwake @be  opone of t,ions /*!< e used ins erived from thoooex-----dflag isn side.T**   SSLEEPEVEONPEND   (+) SYonnctX_LLE*/
OnPend(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCR_SLEnctX_LLE*/
OnPend_STATIC_INLINE uint32_tEVEONPEND _Issleep-on-ex ItwhenTFLAG  riefutionurceIsp
  * @brief CB_SCR  LL_SSLEEPDEEP LL_S iEVONPENDd in timeout fICK_IsslNTMlc APIs that c**
  * @brief CB_SCR  MCbledFO  (+)edFOilSleepis parameter can be one of t}
Implem STMiistriflag isn side.T**o acceeee LIMNOT LER  (+) o acce ofImplem STMi(
   e ofNTceV  * @shouldval C_CTceurc0x4ASR_TEARMte of bit (1 oourcINT*/
__CLKE_HCLKo acce ofImplem STMiacheable memory attriL4        bute F_LOW CB_o acc,.T**o acc_ LIMNOT LER------>>.T**o acc_ LIMNOT LER-0x04imeout fICK_IsslNTce(uint}
V----nt nt)0x07(====r
  * @ followrnpn to endooryvirevenid****fiMCU iag isn side.T**o acceeeeVARIA COUNTFLAG  o acce ofV----nt(
   e ofNTceV  * @betwe_SC0his l255     :oryvireven0)te of bit (1 oourcINT*/
__CLKE_HCLKo acce ofV----ntacheable memory attriL4        bute F_LOW CB_o acc,.T**o acc_VARIA C------>>.T**o acc_VARIA C-0x04imeout fICK_IsslNTce(uint}
ArchiGB S (+)nt)0x07 iag isn side.T**o acceeeeARCHITECTURELAG  o acce ofArchiGB S (+(
   e ofNTceV  * @shouldval C_CTceurc0xCSR_TEtion (SC0+llovicES MPU  bit (1 oourcINT*/
__CLKE_HCLKo acce ofArchiGB S (+acheable memory attriL4        bute F_LOW CB_o acc,.T**o acc_ARCHITECTURE------>>.T**o acc_ARCHITECTURE-0x04imeout fICK_IsslNTce(uint}
Part nt)0x0 iag isn side.T**o acceeeeITY N_MPU_REGICLKo acce ofParNo(
   e ofNTceV  * @shouldval C_CTceurc0xC60SR_TEtion (SC0+ MPU  bit (1 oourcINT*/
__CLKE_HCLKo acce ofParNoacheable memory attriL4        bute F_LOW CB_o acc,.T**o acc_ITY N_------>>.T**o acc_ITY N_-0x04imeout fICK_IsslNTce(uint}
Ryvirevennt)0x07(====p
  * @ followrnpn to endooryvirevenid****fiMC,e tdmeoue  * tchorynt3seU iag isn side.T**o acceeeeREVISnt32_REGICLKo acce ofRyvireve(
   e ofNTceV  * @betwe_SC0his l255    1: * tcho1)te of bit (1 oourcINT*/
__CLKE_HCLKo acce ofRyvireveacheable memory attriL4        bute F_LOW CB_o acc,.T**o acc_REVISnt3------>>.T**o acc_REVISnt3-0x04imeout fICK_IsslNTsource. */
/**
  * @}
  */at c**
  * @brief CB_SCR  MPUS_BUilSleepis parameter can be one of ting valMPUSer mainpusslps CORTEX Expn side.M_CTRL_H_REGIPU_INSTRU_REGICLKM_CTing vaL_SYSTICK_SetCOps CORce
  * @param  Source This parameter can be one of the following values:
  *M_CTRL_H/
#define LL_MPU_C the following values:
  *M_CTRL_H/
#define LL_ the following values:
  *M_CTRL_H/
#define LL_MPU_C the following values:
  *M_CTRL_H/
#define LL_(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCRM_CTing vaL_SYSTICK_Ops CORIC_INLINE uing valos)) /*rceIspWRITEnly)FNMI_MODE LOFNMI_PRIVDPU_INS    (MPOps CORISYSTICE uins (+)MPUSs_ttPDEs take effec) /*!<  *
DSB(SYSTICE uSC_CInc @ fU_RASR_XN_Msk  /*!usn beup@verss_ttPDEs *!<  *
ISB(SYSTcan be one of the following va_BUilSleepn side.M_CTRL_H_REGIPU_INSTRU_REGICLKM_CTowing v(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCRM_CTowing v_STATIC_INLINE uMake s (+)outts CddititransfI, anabldThis*!<  *
DMB(SYSTICE uowing va_BUrceIspWRITEnly)FNMI_MODE LO0UurceIsprivildITis parameter c *@notMPUStimeout functiterilSleepn side.M_CTRL_H_REGIPU_INSTRU_REGICLKM_CT k   /*!<)
{
 e of bit (1 oourcINT*/
_ State of bit (1 oourcINT*/
__CLKE_HCLKM_CT k   /*!<acheable memory attribute F_LONMI_MODE LONMI_PRIVDPU_INS    ------NMI_PRIVDPU_INS    -r can be one of the following valatMPUSof theilSleepn side.M_CTFERA_REGIPU_INSTRU_REGICLKM_CTing vaRf theilSleepCK_SetCEGION_Pe
  * @param  Source This parameter can be one of the following values:
  *M_CT
  */
#define the following values:
  *M_CT
  */
#defin1 the following values:
  *M_CT
  */
#defin2 the following values:
  *M_CT
  */
#defin3 the following values:
  *M_CT
  */
#defin4 the following values:
  *M_CT
  */
#defin5 the following values:
  *M_CT
  */
#defin6 the following values:
  *M_CT
  */
#defin7(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCRM_CTing vaEGION_L_SYSTICK_EGION_NLINE uint32_t EGION_Pnt)0x0UrceIspWRITEnly)FNMI_RNCR _EGION_SYSTICE uing valos)) /e of the MPUIsprivildIFNMI_RFER,SS_BUFFERADPU_INS    timeout fICK_IsslNTce(uint32_tCB->+) MPU API taUSof theilSleepn side.M_CTN   SSLE((uint3TRU_REGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTBAR SSLE((uint3TRU_REGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTBAR SSLEADDR the doooGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REGXN   the doooGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REGAP   the doooGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REG LL_MPU_REGIOGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REGCLL_MPU_REGIOGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REGBLL_MPU_REGIOGICLKM_Ct32_tC_EGION\oer in the dooo.M_CTFERA_REGup COPU_REGIOGICLKM_Ct32_tC_EGIONeilSleepCK_SetCEGION_Pe
  * @param  Source This parameter can be one of the following values:
  *M_CT
  */
#define the following values:
  *M_CT
  */
#defin1 the following values:
  *M_CT
  */
#defin2 the following values:
  *M_CT
  */
#defin3 the following values:
  *M_CT
  */
#defin4 the following values:
  *M_CT
  */
#defin5 the following values:
  *M_CT
  */
#defin6 the following values:
  *M_CT
  */
#defin7(
   pCK_SetAddrvileeV  * s p of thebt3s addrvil7(
   pCK_SetSubCEGIONlowing vaub- of theions /*p
  * @betwe_SMin_Data = RABL+) MPMax_Data = RAFF7(
   pCK_SetANot BuffORce
  * @param  Sourcea com 2.  /*!< Enarameter can be one of the following values:
  *M_CT
  */
#define 
_ues:
  *M_CT
  */
#defU_RA
_ues:
  *M_CT
  */
#defU_RAS
_ues:
  *M_CT
  */
#defU_RAS
_ues:
  *M_CT
  */
#defU_RAf the followin A
_ues:
  *M_CT
  */
#defUKAS
_ues:
  *M_CT
  */
#defUKAS
_ues:
  *M_CT
  */
#def4KAS
_ues:
  *M_CT
  */
#def8KAS
_ues:
  *M_CT
  */
#defU_RAf the followin A
_ues:
  *M_CT
  */
#def3UKAS
_ues:
  *M_CT
  */
#def64KAS
_ues:
  *M_CT
  */
#defU_RASR
_ues:
  *M_CT
  */
#defU_RASR
_ues:
  *M_CT
  */
#defU_RASf the followin A
_ues:
  *M_CT
  */
#defUMSR
_ues:
  *M_CT
  */
#defUMSR
_ues:
  *M_CT
  */
#def4MSR
_ues:
  *M_CT
  */
#def8MSR
_ues:
  *M_CT
  */
#defU_RAf the followin A
_ues:
  *M_CT
  */
#def3UMSR
_ues:
  *M_CT
  */
#def64MSR
_ues:
  *M_CT
  */
#defU_RASR
_ues:
  *M_CT
  */
#defU_RASR
_ues:
  *M_CT
  */
#defU_RASf the followin A
_ues:
  *M_CT
  */
#defUGSR
_ues:
  *M_CT
  */
#defUGSR
_ues:
  *M_CT
  */
#def4GB6 the following values:
  *M_CT
  */
#define L
_ues:
  *M_CT
  */
#define
_ues:
  *M_CT
  */
#define LL_
_ues:
  *M_CT
  */
#define LLf the followin A
_ues:
  *M_CT
  */
#define
_ues:
  *M_CT
  */
#define LL6 the following values:
  *M_C
  */
#de
_ues:
  *M_Cits */
#de
_ues:
  *M_Cits */
#2e
_ues:
  *M_Cits */
#n4 the following values:
  *M_C
  */
#define LL_MPU_INS_ll_ues:
  *M_C
  */
#define LL_MPU_INS4 the following values:
  *M_C
  */
#define L
_ues:
  *M_Cute */
#define LL_M4 the following values:
  *M_C
  */fine LL_MP
_ues:
  *M_Cute */
#define LL_M4 the following values:
  *M_C
  */fine LL_MPU
_ues:
  *M_Cute */
#define LL_MP7(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCRM_Ct32_tC_EGION_L_SYSTICK_EGION,/
__CLKE_SubCEGIONlowing ,/
__CLKE_Addrvil,/
__CLKE_ANot BuffO_NLINE uint32_t EGION_Pnt)0x0UrceIspWRITEnly)FNMI_RNCR _EGION_SYSTICE 32_tbt3s addrvil0UrceIspWRITEnly)FNMI_RBAR, (Addrvile& RAFFFFFFE0URISYSTICE t32_tCB->+) /eUrceIspWRITEnly)FNMI_RFER,S(S_BUFFERADPU_INS    |tANot BuffOR|tSubCEGIONlowing v     ((uint32RD)(0x04SYSTcan be one of the following vaUSof theilSleepn side.M_CTN   SSLE((uint3TRU_REGICLKM_Clowing _EGION\oer in the dooo.M_CTFERA_REGIPU_INSTRU_REGICLKM_CTowing _EGIONeilSleepCK_SetCEGION_Pe
  * @param  Source This parameter can be one of the following values:
  *M_CT
  */
#define the following values:
  *M_CT
  */
#defin1 the following values:
  *M_CT
  */
#defin2 the following values:
  *M_CT
  */
#defin3 the following values:
  *M_CT
  */
#defin4 the following values:
  *M_CT
  */
#defin5 the following values:
  *M_CT
  */
#defin6 the following values:
  *M_CT
  */
#defin7(
   e ofNTce(uint32_tCB->SCR,    ((uint32_CB_SCRM_CTowing aEGION_L_SYSTICK_EGION_NLINE uint32_t EGION_Pnt)0x0UrceIspWRITEnly)FNMI_RNCR _EGION_SYSTICE lowing vlos)) /e of the MPUIsp
  * @briFNMI_RFER,SS_BUFFERADPU_INS    timeout fICK_IsslNTsource. *//
/**
  * @}
  */
#endif /* __MPU_PRESENT */
/* __MPU_PRESENT */
/* __MPU_PRESENT */M32L0xx_LL_CORTEX_H}cplusplus
e/
/**
  *LL_CORTEX_H
#define SENT */
/////////////////////// (C). IN NO EVE3. Neither the name/////PENDOF FILE////NT 