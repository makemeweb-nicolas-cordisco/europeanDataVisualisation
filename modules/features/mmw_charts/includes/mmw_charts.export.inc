<?php
/**
 * @file
 * MMW Charts export charts function file.
 *
 * @brief All functions for exporting current charts.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Function to export current charts with the selected filter.
 */
function _mmw_charts_export_action() {
  $string = filter_input(INPUT_POST, "exporter");
  $data = json_decode($string, TRUE);

  // Get Json array.
  $node = node_load($data['nid']);
  $data_json = _mmw_charts_json_to_array($node);

  $static_data = _mmw_charts_get_static_data($data['nid']);
  $countries_mapping = $static_data['countryMapping'];

  $library = libraries_load('PHPExcel');
  if (empty($library['loaded'])) {
    watchdog('phpexcel', "Couldn't find the PHPExcel library. Excel export aborted.", array(), WATCHDOG_ERROR);

    return PHPEXCEL_ERROR_LIBRARY_NOT_FOUND;
  }

  $excel = new PHPExcel();
  $excel->removeSheetByIndex(0);
  $countries = array_flip($data["countries"]);
  $j = 0;
  foreach ($data["themes"] as $theme) {
    // Get Data from theme.
    $theme_data = $data_json[$theme];
    // Create new sheet.
    $sheet = new PHPExcel_Worksheet();
    $sheet->getColumnDimension("A")->setAutoSize(TRUE);
    $sheet->setTitle(substr(drupal_html_id($theme_data["title"]), 0, 25) . "...");
    $sheet->getCell("A1")->setValue('Source: PRIMES');
    $i = 0;
    $row_index = 1;
    foreach ($theme_data["data"] as $country => $values) {
      if (isset($countries[$countries_mapping[$country]])) {
        $row_index++;
        // Set headers.
        if ($i === 0) {
          $col = 1;
          foreach ($values as $year => $value) {
            $sheet->setCellValueByColumnAndRow($col, $row_index, $year);
            $col++;
          }
          $row_index++;
        }
        $sheet->getCell("A" . $row_index)->setValue($theme_data["title"] . " : " . $country);
        $col = 1;
        foreach ($values as $value) {
          $sheet->setCellValueByColumnAndRow($col, $row_index, $value);
          $col++;
        }
        $i++;
      }
    }
    // Add sheet to PHPEXcel.
    $excel->addSheet($sheet);
    $j++;
  }
  // We'll be outputting an excel file.
  header('Content-type: application/vnd.ms-excel');

  // It will be called file.xls.
  header('Content-Disposition: attachment; filename="file.xls"');

  // Write file to the browser.
  $obj_writer = PHPExcel_IOFactory::createWriter($excel, 'Excel5');
  $obj_writer->save('php://output');
}
