/*******************************************************************************
* File Name: Pin_VDD.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_VDD_H) /* Pins Pin_VDD_H */
#define CY_PINS_Pin_VDD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_VDD_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_VDD__PORT == 15 && ((Pin_VDD__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pin_VDD_Write(uint8 value);
void    Pin_VDD_SetDriveMode(uint8 mode);
uint8   Pin_VDD_ReadDataReg(void);
uint8   Pin_VDD_Read(void);
void    Pin_VDD_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin_VDD_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin_VDD_SetDriveMode() function.
     *  @{
     */
        #define Pin_VDD_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin_VDD_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin_VDD_DM_RES_UP          PIN_DM_RES_UP
        #define Pin_VDD_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin_VDD_DM_OD_LO           PIN_DM_OD_LO
        #define Pin_VDD_DM_OD_HI           PIN_DM_OD_HI
        #define Pin_VDD_DM_STRONG          PIN_DM_STRONG
        #define Pin_VDD_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pin_VDD_MASK               Pin_VDD__MASK
#define Pin_VDD_SHIFT              Pin_VDD__SHIFT
#define Pin_VDD_WIDTH              1u

/* Interrupt constants */
#if defined(Pin_VDD__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin_VDD_SetInterruptMode() function.
     *  @{
     */
        #define Pin_VDD_INTR_NONE      (uint16)(0x0000u)
        #define Pin_VDD_INTR_RISING    (uint16)(0x0001u)
        #define Pin_VDD_INTR_FALLING   (uint16)(0x0002u)
        #define Pin_VDD_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pin_VDD_INTR_MASK      (0x01u) 
#endif /* (Pin_VDD__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_VDD_PS                     (* (reg8 *) Pin_VDD__PS)
/* Data Register */
#define Pin_VDD_DR                     (* (reg8 *) Pin_VDD__DR)
/* Port Number */
#define Pin_VDD_PRT_NUM                (* (reg8 *) Pin_VDD__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_VDD_AG                     (* (reg8 *) Pin_VDD__AG)                       
/* Analog MUX bux enable */
#define Pin_VDD_AMUX                   (* (reg8 *) Pin_VDD__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_VDD_BIE                    (* (reg8 *) Pin_VDD__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_VDD_BIT_MASK               (* (reg8 *) Pin_VDD__BIT_MASK)
/* Bypass Enable */
#define Pin_VDD_BYP                    (* (reg8 *) Pin_VDD__BYP)
/* Port wide control signals */                                                   
#define Pin_VDD_CTL                    (* (reg8 *) Pin_VDD__CTL)
/* Drive Modes */
#define Pin_VDD_DM0                    (* (reg8 *) Pin_VDD__DM0) 
#define Pin_VDD_DM1                    (* (reg8 *) Pin_VDD__DM1)
#define Pin_VDD_DM2                    (* (reg8 *) Pin_VDD__DM2) 
/* Input Buffer Disable Override */
#define Pin_VDD_INP_DIS                (* (reg8 *) Pin_VDD__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_VDD_LCD_COM_SEG            (* (reg8 *) Pin_VDD__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_VDD_LCD_EN                 (* (reg8 *) Pin_VDD__LCD_EN)
/* Slew Rate Control */
#define Pin_VDD_SLW                    (* (reg8 *) Pin_VDD__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_VDD_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_VDD__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_VDD_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_VDD__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_VDD_PRTDSI__OE_SEL0        (* (reg8 *) Pin_VDD__PRTDSI__OE_SEL0) 
#define Pin_VDD_PRTDSI__OE_SEL1        (* (reg8 *) Pin_VDD__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_VDD_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_VDD__PRTDSI__OUT_SEL0) 
#define Pin_VDD_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_VDD__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_VDD_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_VDD__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pin_VDD__SIO_CFG)
    #define Pin_VDD_SIO_HYST_EN        (* (reg8 *) Pin_VDD__SIO_HYST_EN)
    #define Pin_VDD_SIO_REG_HIFREQ     (* (reg8 *) Pin_VDD__SIO_REG_HIFREQ)
    #define Pin_VDD_SIO_CFG            (* (reg8 *) Pin_VDD__SIO_CFG)
    #define Pin_VDD_SIO_DIFF           (* (reg8 *) Pin_VDD__SIO_DIFF)
#endif /* (Pin_VDD__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin_VDD__INTSTAT)
    #define Pin_VDD_INTSTAT            (* (reg8 *) Pin_VDD__INTSTAT)
    #define Pin_VDD_SNAP               (* (reg8 *) Pin_VDD__SNAP)
    
	#define Pin_VDD_0_INTTYPE_REG 		(* (reg8 *) Pin_VDD__0__INTTYPE)
#endif /* (Pin_VDD__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_VDD_H */


/* [] END OF FILE */
