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

<div class="select-row">
    <div id="themes">
            <?php
            foreach ($menu as $item)
            {
                $style = ' style="display:none;"';
                $checked = "";
                if(in_array($item['file'], $themes)){
                    $checked = "checked";
                }
                if($display_type == $item['file']){
                    $style = '';
                }
                echo '<div class="main-theme-container ' . $item['file'] . '"' . $style . '><div class="themes-col">';
                // If no data for parent do not display parent checkbox.
                if(!$item["no-data"]){
                    echo '<div class="menu-lvl1 checkbox checkbox-primary">'
                        . '<input id="theme-' . $item['file'] . '" type="checkbox" name="themes[]" value="' . $item['file'] . '"' . $checked . '>'
                        . '<label for="theme-' . $item['file'] . '">' . $item['name'] . '</label></div>';
                    $checked = '';
                }
                $c = 0;
                foreach ($item['children'] as $item2)
                {
                    // Column creation.
                    if(floor(count($item['children']) / 2) == $c){
                        echo '</div><div class="themes-col">';
                    }
                    $c++;
                    // lvl3 button initialisation if children exist.
                    $children_button = "";
                    if(count($item2['children']) > 0){
                        $children_button = '<button class="lvl3-button">+</button>';
                    }
                    if(!$item2["no-data"]){
                        $checked = "";
                        if(in_array($item2['file'], $themes)){
                            $checked = "checked";
                        }
                        echo '<div class="menu-lvl2 checkbox checkbox-primary">'
                            . '<input id="theme-' . $item2['file'] . '" type="checkbox" name="themes[]" value="' . $item2['file'] . '"' . $checked . '>'
                            . '<label for="theme-' . $item2['file'] . '">' . $item2['name'] . '</label>' . $children_button . '</div>';
                    }

                    $checked = '';
                    foreach ($item2['children'] as $item3)
                    {
                        if(!$item3["no-data"]){
                            $checked = "";
                            if(in_array($item3['file'], $themes)){
                                $checked = "checked";
                            }
                            echo '<div class="menu-lvl3 checkbox checkbox-primary" style="display:none;">'
                            . '<input id="theme-' . $item3['file'] . '" type="checkbox" name="themes[]" value="' . $item3['file'] . '"' . $checked . '>'
                            . '<label for="theme-' . $item3['file'] . '">' . $item3['name'] . '</label></div>';
                        }
                    }
                }
                echo '</div></div>';
            }
            ?>
    </div>
</div>
