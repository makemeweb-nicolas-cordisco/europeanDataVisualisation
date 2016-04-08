<?php
/**
 * @file
 * MMW Charts theme implementation to select_countries field.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */
?>

<div class="right">
    <div id="map-chart" >
        <?php if($commission): ?>
            <script type="application/json">{
                "service" : "charts",
                "custom" : "<?php echo $mini_map_js; ?>"
            }</script>
        <?php endif; ?>
    </div>
    <div id="country-select">
        <?php
        $c = 0;
        $checked = '';
        foreach ($countries as $country_key => $country)
        {
            if($country_key != 'eu27'){
                if($countries_selected == "all"){
                    // All but not eu28.
                    if($country_key != "eu28"){
                        $checked = 'checked';
                    }
                }else{
                    $checked = '';
                    if(in_array($country_key, $countries_selected)){
                        $checked = 'checked';
                    }
                }

                if(!in_array($country_key, $countries_excludes)){
                    echo '<div class="checkbox checkbox-primary ' . $country_key . '"><input id="country-check-' . $country_key . '" type="checkbox" name="countries[]" value="' . $countries_mapping[$country_key] . '" ' . $checked . ' data-color="' . $colors[$country_key] . '">'
                    . '<label data-color="' . $colors[$country_key] . '" for="country-check-' . $country_key . '">' . $country . '</label></div>';
                }

                if($c == 0){
                    $checked = '';
                    if($countries_selected == "all"){
                        $checked = 'checked';
                    }
                    echo '<div class="checkbox checkbox-primary"><input id="all-countries" type="checkbox" name="countries" value="all" ' . $checked . '>'
                    . '<label for="all-countries">Select All Countries</label></div>';
                }
                $c++;
            }
        }

        ?>
    </div>
</div>
