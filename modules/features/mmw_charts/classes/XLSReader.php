<?php
/**
 * @file
 * MMW Charts Class XLS Reader.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Class for read and parse Excel file.
 */
class XLSReader {

  /**
   * Excel reader.
   *
   * @var PHPExcel_Reader_Excel2007
   */
  private $reader = NULL;

  /**
   * Excel sheets.
   *
   * @var Array
   */
  private $sheets = array();

  /**
   * String filename.
   *
   * @var Array
   */
  private $filename = "";

  /**
   * File to be process.
   *
   * @var PHPExcel
   */
  private $file = FALSE;

  /**
   * Data coming from sheet with title ended by "S".
   *
   * @var array
   */
  private $sData = array();

  /**
   * Data coming from sheet with title ended by "I".
   *
   * @var array
   */
  private $iData = array();

  /**
   * Countries found in xls.
   *
   * @var array
   */
  private $countries = array();

  /**
   * Years found in xls.
   *
   * @var array
   */
  private $years = array();

  /**
   * Final array of data.
   *
   * @var array
   */
  public $outputJson = array();

  /**
   * Create new reader.
   */
  public function __construct() {

    $library = libraries_load('PHPExcel');

    if (empty($library['loaded'])) {
      watchdog('phpexcel', "Couldn't find the PHPExcel library. Excel export aborted.", array(), WATCHDOG_ERROR);

      return PHPEXCEL_ERROR_LIBRARY_NOT_FOUND;
    }
    $cache_method = PHPExcel_CachedObjectStorageFactory:: cache_to_phpTemp;
    $cache_settings = array(' memoryCacheSize ' => '8MB');
    PHPExcel_Settings::setCacheStorageMethod($cache_method, $cache_settings);

    $this->reader = new PHPExcel_Reader_Excel2007();
    $this->reader->setReadDataOnly(TRUE);
  }

  /**
   * Open the given filename.
   *
   * @param string $filename
   *   File name.
   *
   * @throws Exception
   */
  public function open($filename = "") {
    // Filename must be provided.
    if (empty($filename)) {
      throw new Exception("Filename could not be empty.");
    }
    // File must exists and be readable.
    elseif (!is_file($filename) || !is_readable($filename)) {
      throw new Exception($filename . " does not exists or is unreadable.");
    }
    $this->filename = $filename;

    $this->sheets = $this->reader->listWorksheetInfo($filename);
  }

  /**
   * Sort XLS data to match requirements in graph.
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   */
  private function sort(PHPExcel_Worksheet $sheet, $sheet_index) {
    $sheet_name = $sheet->getTitle();
    // Only take valid files : {iso}-{i or s}.
    if (preg_match("#^[a-zA-Z0-9]{2,4}\-(A|B)$#", $sheet_name)) {

      // Extract headers.
      if ($sheet_index === 3) {
        $this->extractHeaders($sheet);
      }

      // Extract Data.
      $this->extractData($sheet);
    }
    if (preg_match("#^[a-zA-Z0-9]{2,4}\-(B)$#", $sheet_name)) {
      // Extract country.
      if ($sheet_index > 2) {
        $this->extractCountry($sheet);
      }
    }

    // Definition sheet.
    if ($sheet_index == 2 || $sheet_index == 1) {
      $type = explode('-', $sheet_name);
      $this->extractThemes($sheet, $type[1]);
    }
  }

  /**
   * Create themes for the JSON file.
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   */
  private function extractThemes(PHPExcel_Worksheet $sheet, $type) {
    foreach ($sheet->getRowIterator() as $row) {
      /* @var $row PHPExcel_Worksheet_Row */
      $index = $row->getRowIndex();

      if ($index > 2) {
        $this->extractTheme($sheet, $row, $type);
      }
    }
  }

  /**
   * Create all headers needed for the JSON file.
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   */
  private function extractHeaders(PHPExcel_Worksheet $sheet) {
    foreach ($sheet->getRowIterator() as $row) {
      /* @var $row PHPExcel_Worksheet_Row */
      $index = $row->getRowIndex();

      // Years headers.
      if ($index === 2) {
        $this->extractYears($row);
      }
    }
  }

  /**
   * Extract all years from headers row .
   *
   * @param PHPExcel_Worksheet_Row $row
   *   Excel row.
   */
  private function extractYears(PHPExcel_Worksheet_Row $row) {
    foreach ($row->getCellIterator() as $cell) {
      /* @var $cell PHPExcel_Cell */
      if (is_numeric($cell->getValue())) {
        $c_index = $cell->getColumn();
        $this->years[$c_index] = $cell->getValue();
      }
    }
  }

  /**
   * Extract country from headers row .
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   */
  private function extractCountry(PHPExcel_Worksheet $sheet) {
    // Extract country name.
    $cell_string = $sheet->getCell('P1')->getValue();
    $arr = explode(":", $cell_string, 2);
    $country_name = $arr[0];
    // Extract country code.
    $country_infos = explode('-', $sheet->getTitle(), 2);
    if (isset($country_infos[0])) {
      $country_code = drupal_strtolower(drupal_html_id($country_infos[0]));
      $this->countries[$country_code] = $country_name;
    }
  }

  /**
   * Extract all years from headers row .
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   * @param PHPExcel_Worksheet_Row $row
   *   Excel row.
   */
  private function extractTheme(PHPExcel_Worksheet $sheet, PHPExcel_Worksheet_Row $row, $type) {
    $index = $row->getRowIndex();
    $cell_a = $sheet->getCell("A" . $index);
    $value = $cell_a->getValue();
    $formated_value = $cell_a->getFormattedValue();

    // Only take filled cells.
    $hierarchy = 0;
    if (!empty($value)) {
      $hierarchy = 1;
    }
    else {
      $cell_b = $sheet->getCell("B" . $index);
      $value = $cell_b->getValue();
      $formated_value = $cell_b->getFormattedValue();
      if (!empty($value)) {
        $hierarchy = 2;
      }
      else {
        $cell_c = $sheet->getCell("C" . $index);
        $value = $cell_c->getValue();
        $formated_value = $cell_c->getFormattedValue();
        if (!empty($value)) {
          $hierarchy = 3;
        }
        else {
          $cell_d = $sheet->getCell("D" . $index);
          $value = $cell_d->getValue();
          $formated_value = $cell_d->getFormattedValue();
          if (!empty($value)) {
            $hierarchy = 4;
          }
        }
      }
    }
    // No data in row.
    $cell_f = $sheet->getCell("F" . $index);
    $value_f = $cell_f->getValue();
    $no_data = ($value_f == 'no') ? TRUE : FALSE;
    // Limited data.
    $limited_data = (trim($value_f) == 'limited') ? TRUE : FALSE;

    // Unit.
    $cell_e = $sheet->getCell("E" . $index);
    $unit = $cell_e->getValue();

    if ($hierarchy) {
      $prefix = "s_" . $index;
      $this->sData[$prefix . '_' . drupal_strtolower(drupal_html_id($value))] = array(
        "title" => check_plain($formated_value),
        "rowIndex" => $index,
        "hierarchy" => $hierarchy,
        "sheet_type" => $type,
        "key" => $prefix . '_' . drupal_strtolower(drupal_html_id($value)),
        "data" => array(),
        "no-data" => $no_data,
        "limited-data" => $limited_data,
        "unit" => $unit,
      );
    }
  }

  /**
   * Extract data for the current active sheet.
   *
   * @param PHPExcel_Worksheet $sheet
   *   Excel sheet.
   */
  private function extractData(PHPExcel_Worksheet $sheet) {
    $sanitized_name = preg_replace("#\-(a|b)$#", "", drupal_html_id($sheet->getTitle()));
    $string_exploded = explode("-", $sheet->getTitle());
    $type = $string_exploded[1];
    // Bind the iData set if the file is *-I file.
    if ($sanitized_name != 'eu27') {
      foreach ($this->sData as $file => $theme) {
        // Only theme present on this sheet.
        if ($type == $theme["sheet_type"]) {
          $row_index = $theme["rowIndex"];
          $this->sData[$file]["data"][$sanitized_name] = array();
          foreach ($this->years as $column_index => $year) {
            $value = $sheet->getCell($column_index . $row_index)->getFormattedValue();
            $this->sData[$file]["data"][$sanitized_name][$year] = $value;
          }
        }
      }
    }
  }

  /**
   * Create all JSONs files ready to be displayed by Fusionchart.
   */
  public function createJsonFromFile() {
    // Foreach sheet sort its data.
    foreach ($this->sheets as $sheet_index => $sheet_data) {
      $this->reader->setLoadSheetsOnly(array($sheet_data['worksheetName']));
      // Load file with PHPExcel.
      $this->file = $this->reader->load($this->filename);

      // If load is not successfull throw exception.
      if (!$this->file instanceof PHPExcel) {
        throw new Exception("Failed to load file : " . $this->filename);
      }

      $sheet = $this->file->setActiveSheetIndex(0);
      $this->sort($sheet, $sheet_index);
      $this->file->disconnectWorksheets();
      unset($this->file);
      unset($sheet);
    }
    // Foreach data key, create new json file for all *-S sheets.
    $s_menu = array();
    $s_previous_parent_h1 = NULL;
    $s_previous_parent_h2 = NULL;
    $s_previous_parent_h3 = NULL;

    foreach ($this->sData as $key => $data) {
      $this->extractMenu($s_menu, $key, $data, $s_previous_parent_h1, $s_previous_parent_h2, $s_previous_parent_h3);
      $this->outputJson[$key] = $data;
    }

    $this->outputJson['sMenu'] = $s_menu;

    // Some little operation on country array.
    if (array_key_exists("eu27", $this->countries)) {
      unset($this->countries['eu27']);
    }
    asort($this->countries);
    if (array_key_exists("eu28", $this->countries)) {
      unset($this->countries['eu28']);
      $this->countries = array_merge(array('eu28' => 'European Union'), $this->countries);
    }
    $this->outputJson['countries'] = $this->countries;
    $this->outputJson['years'] = $this->years;
  }

  /**
   * Extract the menu key and place it hierarchically inside the table.
   *
   * @param array $menu
   *   Menu data.
   * @param string $file_key
   *   Current data theme.
   * @param array $data
   *   Current theme data.
   * @param string $previous_parent_h1
   *   Level1 data theme parent.
   * @param string $previous_parent_h2
   *   Level2 data theme parent.
   */
  private function extractMenu(array &$menu, $file_key, array $data, &$previous_parent_h1, &$previous_parent_h2, &$previous_parent_h3) {
    $hierarchy = $data["hierarchy"];
    if ($hierarchy === 1) {
      $menu[$file_key] = array(
        "name" => $data["title"],
        "file" => $file_key,
        "hierarchy" => $hierarchy,
        "no-data" => $data["no-data"],
        "limited-data" => $data["limited-data"],
        "unit" => $data["unit"],
        "children" => array(),
      );
      $previous_parent_h1 = $file_key;
    }
    elseif ($hierarchy === 2) {
      $menu[$previous_parent_h1]["children"][$file_key] = array(
        "name" => $data["title"],
        "file" => $file_key,
        "hierarchy" => $hierarchy,
        "no-data" => $data["no-data"],
        "limited-data" => $data["limited-data"],
        "unit" => $data["unit"],
        "children" => array(),
      );
      $previous_parent_h2 = $file_key;
    }
    elseif ($hierarchy === 3) {
      $menu[$previous_parent_h1]["children"][$previous_parent_h2]["children"][$file_key] = array(
        "name" => $data["title"],
        "file" => $file_key,
        "hierarchy" => $hierarchy,
        "no-data" => $data["no-data"],
        "limited-data" => $data["limited-data"],
        "unit" => $data["unit"],
        "children" => array(),
      );
      $previous_parent_h3 = $file_key;
    }
    elseif ($hierarchy >= 4) {
      $menu[$previous_parent_h1]["children"][$previous_parent_h2]["children"][$previous_parent_h3]["children"][$file_key] = array(
        "name" => $data["title"],
        "file" => $file_key,
        "hierarchy" => $hierarchy,
        "no-data" => $data["no-data"],
        "limited-data" => $data["limited-data"],
        "unit" => $data["unit"],
        "children" => array(),
      );
    }
  }

  /**
   * Return all sheets for the given file.
   *
   * @return array
   *   Return an array of all sheets for the Excel file.
   */
  private function getSheets() {
    return $this->file->getAllSheets();
  }

  /**
   * Return the output Json.
   *
   * @return json
   *   Return an array in json format for the Excel file.
   */
  public function getOutputJson() {
    return $this->outputJson;
  }

}
