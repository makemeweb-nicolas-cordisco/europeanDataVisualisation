INTRODUCTION
------------

Makemeweb Charts is a module for the European Commission.
For the "Advanced Fusioncharts" content type, it renders
a Fusioncharts display on full display node.
The chart is rendered from json data, extracted from an Excel file
uploaded to the Drupal node.

 * For a full description of Fusioncharts, visit the project page:
   http://www.fusioncharts.com/

REQUIREMENTS
------------

This module requires the following modules:

 * Number (https://drupal.org/project/number)
 * Libraries (https://drupal.org/project/libraries)
 * Jquery update (https://drupal.org/project/jquery_update)

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * After the installation is completed, install the PHPExcel libraries.
   You must first download the entire library (1.8.*) and put it under
   sites/*/libraries/PHPExcel/ (so you should have 
   sites/*/libraries/PHPExcel/Classes/PHPExcel.php). 
   Make sure to include the changelog.txt file as well 
   (sites/*/libraries/PHPExcel/changelog.txt).
   For a full description of PHPExcel, visit the project page: 
   https://github.com/PHPOffice/PHPExcel
