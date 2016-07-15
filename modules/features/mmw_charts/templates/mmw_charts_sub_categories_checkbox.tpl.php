<?php

/**
 * @file
 * MMW Charts theme implementation to sub_categories_checkbox field.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */
?>

<?php $c = 0; ?>
<?php foreach ($elements as $element): ?>
  <?php if($element['level'] == 3 && floor(count($elements) / 2) == $c): ?>
    </div></div><div class="col-sm-6">
        <div class="row">
  <?php endif; ?>
  <?php $c++; ?>
  <div class="menu-lvl<?php print $element['level']; ?> checkbox checkbox-primary" <?php if($element['level'] == 4): ?>style="display: none;"<?php
 endif; ?>>
    <?php if(!$element['no-data']): ?>
      <input id="theme-<?php print $element['file']; ?>" data-unit="<?php print $element['unit']; ?>" data-limited="<?php print $element['limited-data']; ?>" data-description="<?php print $element['description']; ?>" type="checkbox" name="themes[]" value="<?php print $element['file']; ?>" <?php print ' ' . $element['checked']; ?>>
    <?php endif; ?> 
    <label for="theme-<?php print $element['file']; ?>"><?php print $element['name']; ?></label>
    <?php if(!empty($element['button'])): ?>
      <button class="lvl<?php print $element['level'] + 1; ?>-button">+</button>
    <?php endif; ?>
  </div>
  <?php if(!empty($element['children'])): ?>
    <?php print theme('mmw_charts_sub_categories_checkbox', array('elements' => $element['children'])); ?>
  <?php endif; ?>
<?php endforeach; ?>
