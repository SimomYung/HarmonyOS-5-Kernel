# About This Reference

This document mainly describes C library interface functions for Hongmeng Kernel, each of which mainly contains the following content:

   * Synopsis 
   * Arguments 
   * Library
   * Description
   * Returns
   * Errors
   * Examples
   * Classification
   * Function safety
   * See also
 
## Synopsis:  
  This section describes the list of parameters that the function takes and the header files that are included and declared.
 
## Arguments:  
  This section introduces the parameters of the function and related special structures.
 
## Library:  
  This section introduces the libraries that need to be referenced and linked in order to use this function.
 
## Description:
This section describes the function or macro.
 
## Returns:
This section describes the return values of functions.

## Errors:
This section provides a list of values that may be stored in the global errno in case of an error occurs, with information about why errors happened.  

&#8195; &#8195; \* This section does not necessarily list all errno values set by this function.  

## Example:
This section provides a demonstration use case for this feature, which is usually just a code snippet and not a complete feature.

## Classification:
This chapter is usually used to describe the source of the function definition and its corresponding standard, such as Posix.1003.1-2017 and Native Interface:  
&nbsp;  

- **Native Interface**  
These functions or macros are not POSIX, only perform a function related to our OS.  
&nbsp;  

- **POSIX.1003.1-2017**  
IEEE Std 1003.1-2017 Portable Operating System Interface (POSIX(R)) Base Specifications, Issue 7 is available from either The Open Group or IEEE and is, as of 22.7.2018, the current standard.  
&nbsp;  

- **Unix**  
This means that the implementation of this function refers to some Unix or Unix-like systems, but does not mean that it has passed Unix certification.  

## Function safety:
This section describes whether the use of this function is functional safety compliant in some cases:  
&nbsp;  

- **Cancel-Safe**  	
Cancel-Safe functions are safe to call when asynchronous cancellation is enabled.  
&nbsp;  

- **Interrupt-Safe**  
Interrupt-Safe functions are safe to use in an interrupt handler. If a function isn't flagged as interrupt-safe, don't use it in an interrupt handler.  
&nbsp;  

- **Signal-Safe**  
Signal-Safe functions are safe to call in the signal handler.  
&nbsp;  

- **Thread-Safe**  
Thread-Safe functions are safe to call in the presence of thread or other threads. It is the same as MT-Safe. MT stands for Multi Thread.  
&nbsp;  

- **Automotive Safety Integrity Level (ASIL)**  
Automotive Safety Integrity Level (ASIL) is a risk classification scheme defined by the ISO 26262 - Functional Safety for Road Vehicles standard. This is an adaptation of the Safety Integrity Level used in IEC 61508 for the automotive industry. This classification helps defining the safety requirements necessary to be in line with the ISO 26262 standard. The ASIL is established by performing a risk analysis of a potential hazard by looking at the Severity, Exposure and Controllability of the vehicle operating scenario. The safety goal for that hazard in turn carries the ASIL requirements.  
&nbsp;  
There are four ASILs identified by the standard: ASIL A, ASIL B, ASIL C, ASIL D. ASIL D dictates the highest integrity requirements on the product and ASIL A the lowest. Hazards that are identified as QM do not dictate any safety requirements.  (This comment is from the WiKi)  
&nbsp;  

- **Example:**  

 Function Safety		 	&#8195;|&#8195; Status   
 :-------------				       |:-------------  
 Cancel-Safe			 	&#8195;|&#8195; Yes  
 Interrupt-Safe			 	&#8195;|&#8195; No  
 Signal-Safe     		 	&#8195;|&#8195; No   
 Thread-Safe     		 	&#8195;|&#8195; No  
 Automotive Safety Integrity Level 	&#8195;|&#8195; ASIL-D  

## See also:
This optional section provides a list of functions and information related to this function for more information.

