/*******************************************************************************
* File Name: UART_TERMINAL_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_TERMINAL.h"


/***************************************
* Local data allocation
***************************************/

static UART_TERMINAL_BACKUP_STRUCT  UART_TERMINAL_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART_TERMINAL_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the UART_TERMINAL_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_TERMINAL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_TERMINAL_SaveConfig(void)
{
    #if(UART_TERMINAL_CONTROL_REG_REMOVED == 0u)
        UART_TERMINAL_backup.cr = UART_TERMINAL_CONTROL_REG;
    #endif /* End UART_TERMINAL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_TERMINAL_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_TERMINAL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling UART_TERMINAL_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void UART_TERMINAL_RestoreConfig(void)
{
    #if(UART_TERMINAL_CONTROL_REG_REMOVED == 0u)
        UART_TERMINAL_CONTROL_REG = UART_TERMINAL_backup.cr;
    #endif /* End UART_TERMINAL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART_TERMINAL_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The UART_TERMINAL_Sleep() API saves the current component state. Then it
*  calls the UART_TERMINAL_Stop() function and calls 
*  UART_TERMINAL_SaveConfig() to save the hardware configuration.
*  Call the UART_TERMINAL_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_TERMINAL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_TERMINAL_Sleep(void)
{
    #if(UART_TERMINAL_RX_ENABLED || UART_TERMINAL_HD_ENABLED)
        if((UART_TERMINAL_RXSTATUS_ACTL_REG  & UART_TERMINAL_INT_ENABLE) != 0u)
        {
            UART_TERMINAL_backup.enableState = 1u;
        }
        else
        {
            UART_TERMINAL_backup.enableState = 0u;
        }
    #else
        if((UART_TERMINAL_TXSTATUS_ACTL_REG  & UART_TERMINAL_INT_ENABLE) !=0u)
        {
            UART_TERMINAL_backup.enableState = 1u;
        }
        else
        {
            UART_TERMINAL_backup.enableState = 0u;
        }
    #endif /* End UART_TERMINAL_RX_ENABLED || UART_TERMINAL_HD_ENABLED*/

    UART_TERMINAL_Stop();
    UART_TERMINAL_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_TERMINAL_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  UART_TERMINAL_Sleep() was called. The UART_TERMINAL_Wakeup() function
*  calls the UART_TERMINAL_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  UART_TERMINAL_Sleep() function was called, the UART_TERMINAL_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_TERMINAL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_TERMINAL_Wakeup(void)
{
    UART_TERMINAL_RestoreConfig();
    #if( (UART_TERMINAL_RX_ENABLED) || (UART_TERMINAL_HD_ENABLED) )
        UART_TERMINAL_ClearRxBuffer();
    #endif /* End (UART_TERMINAL_RX_ENABLED) || (UART_TERMINAL_HD_ENABLED) */
    #if(UART_TERMINAL_TX_ENABLED || UART_TERMINAL_HD_ENABLED)
        UART_TERMINAL_ClearTxBuffer();
    #endif /* End UART_TERMINAL_TX_ENABLED || UART_TERMINAL_HD_ENABLED */

    if(UART_TERMINAL_backup.enableState != 0u)
    {
        UART_TERMINAL_Enable();
    }
}


/* [] END OF FILE */
