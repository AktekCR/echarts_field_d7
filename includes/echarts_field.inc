<?php


/**
* Created by Randall Sibaja R.
**/
function echarts_field_get_data_from_json($key, $json, $series_x)
{
 
  $output = array();

  foreach($json as $row){
    
    $var = trim($row->$key);

    if(in_array($var, $series_x ))
    {
      $output[] = $var;  
    }
  }
  return $output;
}

/**
* Created by Randall Sibaja R.
**/
function echarts_field_create_polar_radar($key, $json, $series_x, $max_y)
{

  $output = array();
  foreach($json as $row){

    $var = trim($row->$key);

    if(in_array($var, $series_x ))
    {
      $output[] = array(
        'text' => t($var),
        'max' =>  intval($max_y)
      ); 
    }
  }
  return $output;
}


/**
* Created by Randall Sibaja R.
**/
function echarts_field_get_series($xaxis, $series, $data, $graph_type, $series_x, $shadow , $series_type, $stack_bar, $show_average, $show_maxMin)
{

  $output = array();

  if( ($graph_type == 'pie') || ($graph_type == 'funnel') ){
    foreach($data as $row){
      if(in_array($row->$xaxis, $series)){
        $output[] = array(
          'name' => t($row->$xaxis),
          'value' => intval(str_replace(',', '.', $row->$series_x)),
        );
      }
    }

  }else if ( ($graph_type == 'bar') || ($graph_type == 'line') ){
      
    foreach ($series as $key => $v){
      $data_temp = array();
      
      foreach($data as $row){
        $tempS = trim($row->$xaxis);     
        if (in_array($tempS,$series_x)) 
        {
          $tempV = trim($row->$v);
          $data_temp[] = intval(str_replace(',', '.', $tempV));
        }
      }

      $average = array(
        'data' => array(
          array(
          'type' => 'average',
          'name'=> t('Promedio')
          )
        )
      );

     $mark =  array(
        'data' => array(
          array(
            'type' => 'max',
            'name'=> 'Maximo'
          ),
          array(
            'type' => 'min',
            'name' => 'Minimo'
          )
        )
      );

     $dataTemp = array(
        'name' => $v,
        'type'=> $series_type != '-' ? $series_type[$key] : $graph_type,
        'data'=> $data_temp
      );

    if($show_average == 'Si'){
      $dataTemp['markLine'] = $average;
    }

    if($show_maxMin == 'Si'){
      $dataTemp['markPoint'] = $mark;
    }

    if($shadow == 'Si'){
      $dataTemp['smooth'] = true;
      $dataTemp['itemStyle'] = array(
        'normal' => array(
          'areaStyle' => array(
            'type' => 'default'  
          ) 
        )
      );
    }

    if($stack_bar == 'Si'){
      $dataTemp['stack'] = 'total';
      $dataTemp['itemStyle'] = array(
        'normal' => array(
          'label' => array(
            'show' => TRUE,
            'position' => 'insideRight'
          )
        )  
      );
    }

      $output[] = $dataTemp;
    }
  }elseif ($graph_type == 'radar') {
    
    foreach ($series as $key => $v) 
      {
          $data_temp = array();
          foreach($data as $myValue)
          {
            $data_temp[] = intval(str_replace(',', '.',$myValue-> $v));
          }
          $output[] = array('value'=> $data_temp, 'name'=> $v);
      }
  }elseif ($graph_type == 'treemap') {
      foreach($data as $row)
      {
        if(in_array($row->$xaxis, $series))
        {
          $output[] = array('name'=> $row->$xaxis , 'value' => intval($row->$series_x));
        }
      }  
  }
  return $output;  
}

/**
* Created by  Randall Sibaja R.
**/
function echarts_field_generate_settings_bar_line_graph($json, $xaxis, $series, $type, $series_x ,$title = '', $enable_y, $shadow, $series_type,$stack_bar, $show_average, $show_maxMin)
{
  

  $options = array(
    'tooltip' => array('trigger' => 'axis'),          
    'title' => array('text' => $title),
    'legend' => array(
      'data' => $series,
      'orient' => 'horizontal',
    ),
    'toolbox' => array(
        'show' => TRUE,
        'feature' => array(
          'mark' => array(
            'show' => TRUE,
            'title' => array(
              'mark' => t('Marcar'),
              'markUndo' => t('Deshacer'),
              'markClear' => t('Limpiar Marcas')
            )
          ),
          'dataView' => array(
            'show' => TRUE,
            'readOnly' => false,
            'title' => t('Ver Datos'),
            'lang' => array(
              t('Datos'),
              t('Regresar'),
              ''
            )        
          ),
         'magicType' => array(
              'show' => true,
              'type' => array('line','bar'),
              'title' => array(
                'line' => t('Lineal'),
                'bar' => t('Barras')
              )
          ),  
          'restore' => array(
            'show' => TRUE,
            'title' => t('Refrescar')
          ),
          'saveAsImage' => array(
            'show' => TRUE,
            'title' => t('Guardar'),
            'type' => 'png'
          ),
        )
    ),
    'dataZoom' => array(
        'show' => true,
        'realtime' => true,
        'start' => 10,
        'end' => 100
    ),
    'calculable' => true,
    'xAxis' => array(
      '0' => array(
        'type' => 'category',
        'data' => echarts_field_get_data_from_json($xaxis, $json, $series_x)
      )
    ),
    'yAxis' => array(
      '0' => array(
        'type' => 'value',
        'axisLabel' => array(
          'show' => $enable_y == 'Si' ? true : false
        )
      )
    ),
    'series' => echarts_field_get_series($xaxis, $series, $json, $type, $series_x, $shadow, $series_type,$stack_bar, $show_average, $show_maxMin),
  );

  if($stack_bar == 'Si'){
      $xA = $options['xAxis'];
      $yA = $options['yAxis'];
      $options['xAxis'] = $yA;
      $options['yAxis'] = $xA;
  }

  $output = array(
    'chartType' => $type,
    'useType' => array('bar','line'),
    'containerId' => hash('MD5',microtime(true)),
    'options' => $options
  );

  return $output;
}

/**
* Created by Randall Sibaja R.
**/
function echarts_field_generate_settings_pie_funnel_graph($json,$xaxis , $series, $type, $series_x, $title='', $max_y)
{
    
  $chart = array(
    'chartType' => $type,
    'useType' => array('pie','funnel'),
    'containerId' => hash('MD5',microtime(true)),
    'options' => array(
        'tooltip' => array(
          'trigger' => 'item',
          'formatter' => '{a} <br/>{b} : {c} ({d}%)'
        ),      
        'title' => array(
          'text' => $title,
          'subtext' => $series_x,
          'x'=> 'center'
        ),
        'legend' => array(
          'data' => $series,
          'orient' => 'vertical',
          'x' => 'left',
          'y' => 'bottom'
        ),
          'toolbox' => array(
            'show' => TRUE,
            'feature' => array(
              'mark' => array(
                'show' => TRUE,
                'title' => array(
                  'mark' => t('Marcar'),
                  'markUndo' => t('Deshacer'),
                  'markClear' => t('Limpiar Marcas')
                )
              ),
            'dataView' => array(
              'show' => TRUE,
              'readOnly' => false,
              'title' => t('Ver Datos'),
              'lang' => array(
                t('Datos'),
                t('Regresar'),
                ''
              )          
            ),
            'magicType' => array(
              'show' => true,
              'type' => array('pie','funnel'),
              'title' => array(
                'pie' => t('Pie'),
                'funnel' => t('Funnel')
              ),
              'option' => array( 
                'funnel' => array(
                  'x' => '20%',
                  'width' => '50%',
                  'funnelAlign' => 'center'
                )
              )
            ), 
            'restore' => array(
              'show' => TRUE,
              'title' => t('Refrescar')
            ),
            'saveAsImage' => array(
              'show' => TRUE,
              'title' => t('Guardar'),
              'type' => 'png'
            ),
          )
        ),
        'calculable' => TRUE,
        'series' => array(
          array(
              'name'=> t($series_x),
              'type'=> $type,
              'width' => $type == 'funnel' ? '50%' : '80%',
              'x' => $type == 'funnel' ? '20%': '',
              'radius' => '70%',
              'funnelAlign' => $type == 'funnel' ? 'center' : '',
              'max' => $max_y,   
              'center' => array('60%','50%'),
              'data'=> echarts_field_get_series($xaxis, $series, $json, $type, $series_x, '','','', '' ,'')
          )          
        )
      )
    );
  return $chart;
}

/**
*Created by  Randall Sibaja R.
*
**/
function echarts_field_generate_settings_radar_graph($json, $xaxis , $series, $type, $series_x, $max_y, $title='')
{  
  $radar = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => hash('MD5',microtime(true)),
    'options' => array(
        'tooltip' => array('trigger' => 'axis'),          
        'title' => array('text' => $title),
        'legend' => array(
          'data' => $series,
          'orient' => 'horizontal',
          'x' => 'center',
          'y' => 'bottom'
        ),
         'toolbox' => array(
            'show' => TRUE,
            'feature' => array(
              'mark' => array(
                'show' => TRUE,
                'title' => array(
                  'mark' => t('Marcar'),
                  'markUndo' => t('Deshacer'),
                  'markClear' => t('Limpiar Marcas')
                )
              ),
              'dataView' => array(
              'show' => TRUE,
              'readOnly' => false,
              'title' => t('Ver Datos'),
              'lang' => array(
                t('Datos'),
                t('Regresar'),
                ''
              )          
            ),
            'restore' => array(
              'show' => TRUE,
              'title' => t('Refrescar')
            ),
            'saveAsImage' => array(
              'show' => TRUE,
              'title' => t('Guardar'),
              'type' => 'png'
            ),
          )
        ),
        'calculable' => true,
        'polar' => array(
            array(
                'indicator' => echarts_field_create_polar_radar($xaxis,$json,$series_x,$max_y),
                'radius' => '80%'
              )
        ),
        'series' => array(
          array(
              'name' => $title,
              'type' => 'radar',
              'itemStyle' => array(
                'normal'=> array(
                  'areaStyle' => array(
                    'type'=> 'default'
                    )
                  )
              ),
              'data' => echarts_field_get_series($xaxis, $series, $json, $type, $series_x,'','','','','')
          )
        ) 
      )
    );
    return $radar; 
}


/**
*Created by Randall Sibaja R. 
*
**/
function echarts_field_generate_settings_treemap_graph($json,$xaxis , $series, $type, $series_x, $title='')
{
  $treemap = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => hash('MD5',microtime(true)),
    'options' => array(
      'tooltip' => array(
        'trigger' => 'item',
        'formatter' => '{b}: {c}'
      ),      
      'title' => array(
        'text' => $title,
        'subtext' => $series_x,
        'x'=> 'center'
      ),
      'toolbox' => array(
        'show' => TRUE,
        'feature' => array(
          'mark' => array(
            'show' => TRUE,
            'title' => array(
              'mark' => t('Marcar'),
              'markUndo' => t('Deshacer'),
              'markClear' => t('Limpiar Marcas')
              )
          ),
          'dataView' => array(
            'show' => TRUE,
            'readOnly' => false,
            'title' => t('Ver Datos'),
            'lang' => array(t('Datos'), t('Regresar'), '')          
          ),
          'restore' => array(
            'show' => TRUE,
            'title' => t('Refrescar')
          ),
          'saveAsImage' => array(
            'show' => TRUE,
            'title' => t('Guardar'),
            'type' => 'png'
          )
        )
      ),
      'calculable' => false,
      'series' => array(
        array(
          'name'=> t($series_x),
          'type'=> $type,
          'itemStyle' => array(
            'normal' => array(
              'label' => array(
                'show'=>true,
                'formatter' => '{b}'
              ),
              'borderWidth' => 1
            ),
            'emphasis' => array(
              'label' => array(
                'show' => true
              )
            )
          ),
          'data'=> echarts_field_get_series($xaxis, $series, $json, $type, $series_x,'','','','','')
        )          
      )
    )
  );
  return $treemap;
}



