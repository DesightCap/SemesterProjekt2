/*******************************************************************************
* File Name: Pin_SDL.h  
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

#if !defined(CY_PINS_Pin_SDL_H) /* Pins Pin_SDL_H */
#define CY_PINS_Pin_SDL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_SDL_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_SDL__PORT == 15 && ((Pin_SDL__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pin_SDL_Write(uint8 value);
void    Pin_SDL_SetDriveMode(uint8 mode);
uint8   Pin_SDL_ReadDataReg(void);
uint8   Pin_SDL_Read(void);
void    Pin_SDL_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin_SDL_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin_SDL_SetDriveMode() function.
     *  @{
     */
        #define Pin_SDL_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin_SDL_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin_SDL_DM_RES_UP          PIN_DM_RES_UP
        #define Pin_SDL_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin_SDL_DM_OD_LO           PIN_DM_OD_LO
        #define Pin_SDL_DM_OD_HI           PIN_DM_OD_HI
        #define Pin_SDL_DM_STRONG          PIN_DM_STRONG
        #define Pin_SDL_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pin_SDL_MASK               Pin_SDL__MASK
#define Pin_SDL_SHIFT              Pin_SDL__SHIFT
#define Pin_SDL_WIDTH              1u

/* Interrupt constants */
#if defined(Pin_SDL__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin_SDL_SetInterruptMode() function.
     *  @{
     */
        #define Pin_SDL_INTR_NONE      (uint16)(0x0000u)
        #define Pin_SDL_INTR_RISING    (uint16)(0x0001u)
        #define Pin_SDL_INTR_FALLING   (uint16)(0x0002u)
        #define Pin_SDL_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pin_SDL_INTR_MASK      (0x01u) 
#endif /* (Pin_SDL__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_SDL_PS                     (* (reg8 *) Pin_SDL__PS)
/* Data Register */
#define Pin_SDL_DR                     (* (reg8 *) Pin_SDL__DR)
/* Port Number */
#define Pin_SDL_PRT_NUM                (* (reg8 *) Pin_SDL__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_SDL_AG                     (* (reg8 *) Pin_SDL__AG)                       
/* Analog MUX bux enable */
#define Pin_SDL_AMUX                   (* (reg8 *) Pin_SDL__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_SDL_BIE                    (* (reg8 *) Pin_SDL__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_SDL_BIT_MASK               (* (reg8 *) Pin_SDL__BIT_MASK)
/* Bypass Enable */
#define Pin_SDL_BYP                    (* (reg8 *) Pin_SDL__BYP)
/* Port wide control signals */                                                   
#define Pin_SDL_CTL                    (* (reg8 *) Pin_SDL__CTL)
/* Drive Modes */
#define Pin_SDL_DM0                    (* (reg8 *) Pin_SDL__DM0) 
#define Pin_SDL_DM1                    (* (reg8 *) Pin_SDL__DM1)
#define Pin_SDL_DM2                    (* (reg8 *) Pin_SDL__DM2) 
/* Input Buffer Disable Override */
#define Pin_SDL_INP_DIS                (* (reg8 *) Pin_SDL__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_SDL_LCD_COM_SEG            (* (reg8 *) Pin_SDL__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_SDL_LCD_EN                 (* (reg8 *) Pin_SDL__LCD_EN)
/* Slew Rate Control */
#define Pin_SDL_SLW                    (* (reg8 *) Pin_SDL__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_SDL_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_SDL__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_SDL_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_SDL__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_SDL_PRTDSI__OE_SEL0        (* (reg8 *) Pin_SDL__PRTDSI__OE_SEL0) 
#define Pin_SDL_PRTDSI__OE_SEL1        (* (reg8 *) Pin_SDL__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_SDL_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_SDL__PRTDSI__OUT_SEL0) 
#define Pin_SDL_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_SDL__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_SDL_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_SDL__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pin_SDL__SIO_CFG)
    #define Pin_SDL_SIO_HYST_EN        (* (reg8 *) Pin_SDL__SIO_HYST_EN)
    #define Pin_SDL_SIO_REG_HIFREQ     (* (reg8 *) Pin_SDL__SIO_REG_HIFREQ)
    #define Pin_SDL_SIO_CFG            (* (reg8 *) Pin_SDL__SIO_CFG)
    #define Pin_SDL_SIO_DIFF           (* (reg8 *) Pin_SDL__SIO_DIFF)
#endif /* (Pin_SDL__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin_SDL__INTSTAT)
    #define Pin_SDL_INTSTAT            (* (reg8 *) Pin_SDL__INTSTAT)
    #define Pin_SDL_SNAP               (* (reg8 *) Pin_SDL__SNAP)
    
	#define Pin_SDL_0_INTTYPE_REG 		(* (reg8 *) Pin_SDL__0__INTTYPE)
#endif /* (Pin_SDL__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_SDL_H */


/* [] END OF FILE */
