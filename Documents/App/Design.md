# Design

* [Overview](#overview)
* [Views](#views)
* [Configuration](#configuration)
* [Display and Delete Error Codes](#display-and-delete-error-codes)
* [Live Data Views](#live-data-views)

## Overview

1. Bluetooth browse / connection Mask *(Only if not connected to the device)*

2. Show Views *(Show last view selected by user)*

   * Views can be changed by swiping left or right
   
   * View inidcators = On the bottom side of the page is are small circles which shell indicate which View is currently shown.
   
     **Draft:**
     
     <img alt="In Progress" src="https://github.com/Itiapu/HSPSmartCar/blob/APP/Documents/App/Draft/view-indicators.png" width="200">
   
   * Swipe Help = Show a quick, fade out (~2sec) animation that the views can be changed by swiping left or right *(Only if enabled in [Configuration](#configuration))*
   
      **Examples:**

      <img alt="Example 1" src="https://i.pinimg.com/originals/3b/b6/22/3bb62217f2c1e7b6ee1b0ba8d9fc4aac.gif" width="200">
      
      <img alt="Example 2" src="https://thumbs.gfycat.com/PhonyWealthyEastrussiancoursinghounds-max-1mb.gif" width="200">
      
      <img alt="Example 3" src="https://thumbs.gfycat.com/ThoroughFixedHorseshoebat-max-1mb.gif" width="100">  

## Views

Views in order from the left to the right.

### Configuration

* Download Data from Device *(Display 0-100% and Amount of Bytes downloaded e.g. 250 kB of 30.200kB)*

  Afterward Question if the Upload to the Backend shell be done immediately or not yet.
  
  Error if max storage exceeded. *(Upload Data to Backend is needed before downloading more data.)*
  
* Upload Data to Backend *(Display 0-100% and Amount of Bytes uploaded e.g. 250 kB of 30.200kB)*

  Show the amount of buffered Bytes (The not transfered Bytes) permanently.
  
  With bar showing current and maximal Storage (e.g. |#####__| 75% storage used)
  
* Clear Buffer *(Clear not transfered Data - With strict confirm dialog)*

  All downloaded not transfered Data stored in the App will be deleted.
  
* MpH or KmH
  
* Enable / Disable [Swipe Help](#overview)

* URL to Backend

* Login Parameter *(User/Password/Token for Backend)*

* Test Connection to Backend

* Diconnect from Device - Clear Device Storage *(With strict confirm dialog)*

  Disconnecting from Bluetooth.

* Logout

### Live Data Views

<img alt="In Progress" src="https://i.stack.imgur.com/W08Uq.png" width="48">

### Display and Delete Error Codes

<img alt="In Progress" src="https://i.stack.imgur.com/W08Uq.png" width="48">
