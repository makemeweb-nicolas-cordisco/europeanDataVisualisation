<?php
/**
 * @file
 * MMW Charts theme implementation to categories_checkbox field.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */
?>

<div class="keep-float">
  <div id="themes">
    <?php foreach ($elements as $element): ?>
      <div class="main-theme-container <?php print $element['file']; ?> <?php print $element['hidden']; ?>">
        <div class="col-sm-6">
          <div class="row">
            <?php if(!$element['no-data']): ?>
              <div class="menu-lvl<?php print $element['level']; ?> checkbox checkbox-primary">
                <input id="theme-<?php print $element['file']; ?>" data-limited="<?php print $element['limited-data']; ?>" type="checkbox" name="themes[]" value="<?php print $element['file']; ?>" <?php print ' ' . $element['checked']; ?>>
                <label for="theme-<?php print $element['file']; ?>"><?php print $element['name']; ?></label>
              </div>
            <?php endif; ?>
            <?php if(!empty($element['children'])): ?>
              <?php print theme('mmw_charts_sub_categories_checkbox', array('elements' => $element['children'])); ?>
            <?php endif; ?>
          </div>
        </div>
      </div>
    <?php endforeach; ?>
  </div>
</div>
