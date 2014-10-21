<?php


function field_col_load($field_collection_item_id,$limit_point=''){



     $field_collection_item_fields = entity_load('field_collection_item', (array)$field_collection_item_id);

  dpm($field_collection_item_fields);





    //return $file_col_project;

}

function field_col_load_old($node,$limit_point=''){

    $field_collection_items = field_get_items('node', $node, FC_POINT);
    $field_collection_item_ids = array();
    $file_col_project=array();

    if($field_collection_items){
      $i = 0;
      foreach ($field_collection_items as $field_collection_item){
          $field_collection_item_ids[$i] = $field_collection_item['value'];
          $i++;
      }

      /*
       *  load the content of field_collection_items fields "foo" and "bar"
       */

      $field_collection_item_fields = entity_load('field_collection_item', $field_collection_item_ids);


      foreach ($field_collection_item_fields as $field_collection_item_field){
        $point_fields = field_get_items('field_collection_item', $field_collection_item_field, FIELD_NID_ACTION);
        $advanc_fields = field_get_items('field_collection_item', $field_collection_item_field, FIELD_ADVANC);
        //dpr($point_fields) ;
        if(empty($limit_point) || $limit_point == $point_fields[0]['nid']){
          $file_col_project[$field_collection_item_field->item_id]=array(
            'nid_point'=>$point_fields[0]['nid'],
            'tid_advancement'=>$advanc_fields[0]['tid'],
          );

          $prop_fields = field_get_items('field_collection_item', $field_collection_item_field, FC_PROP);
          if($prop_fields){
            $field_collection_prop_item_ids=array();
            foreach ($prop_fields as $prop_field){
                $field_collection_prop_item_ids[] = $prop_field['value'];
            }

            $field_collection_prop_item_fields = entity_load('field_collection_item', $field_collection_prop_item_ids);
            foreach( $field_collection_prop_item_fields as $field_collection_prop_item_field){
              $prop_fields = field_get_items('field_collection_item', $field_collection_prop_item_field, FIELD_NID_PROP);
              $pond_fields = field_get_items('field_collection_item', $field_collection_prop_item_field, FIELD_POND);
              $file_col_project[$field_collection_item_field->item_id]['fc_propos'][$field_collection_prop_item_field->item_id]=array(
                'nid_prop'=>$prop_fields[0]['nid'],
                'tid_pond'=>$pond_fields[0]['tid'],
              );

            }
          }
        }

      }

    }


    return $file_col_project;

}

function in_multiarray($elem, $array){

  foreach($array as $item){
    if(is_array($item) && in_array($elem,$item)){
      return true;
    }

    if(is_array($item) ){
      if(in_multiarray($elem, $item ))   return true;
    }

  }
  return false;
}

function check_array_diff($a1,$a2){
  $r = array();
  if(sizeof($a1) > sizeof($a2)){
    foreach($a1 as $k1=> $v1){
      if(!isset($a2[$k1])){
        $r[]=$v1;
      }
    }
  }
  else{
    foreach($a2 as $k2=> $v2){
      if(!isset($a1[$k2])){
        $r[]=$v2;
      }
    }
  }
  return $r;
}

function array_values_recursive($array,&$vals = array()){
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      array_values_recursive($value,$vals);
    }else{
      $vals[] = $value;
    }
  }
  return $vals;
}

function merge_fields_values($arr1, $arr2,$value='value'){
  if(count($arr1)>0 && $arr2  ){
    foreach($arr2 as $key => $data)  {
      if($arr1 && !in_multiarray($data[$value],$arr1 )){
        $arr1[]=$data;
      }
    }
    return $arr1;
  }
  elseif(count($arr1)==0 && $arr2 && count($arr2)>0  ){
    $arr1=$arr2;
    return $arr1;
  }
  return $arr1;


}

function arrayRecursiveDiff($aArray1, $aArray2) {
    $aReturn = array();

    foreach ($aArray1 as $mKey => $mValue) {
        if (array_key_exists($mKey, $aArray2)) {
            if (is_array($mValue)) {
                $aRecursiveDiff = arrayRecursiveDiff($mValue, $aArray2[$mKey]);
                if (count($aRecursiveDiff)) { $aReturn[$mKey] = $aRecursiveDiff; }
            } else {
                if ($mValue != $aArray2[$mKey]) {
                    $aReturn[$mKey] = $mValue;
                }
            }
        } else {
            $aReturn[$mKey] = $mValue;
        }
    }

    return $aReturn;
}

function _get_icone_content_type($node,$type,$class='icone-type-contenu', $with_style=FALSE){
  $icone_teaser = false;
  switch( $type){
    case 'action' :
      //$field=field_get_items('node', $node, 'field_type_groupe_travail' );
      //$term=taxonomy_term_load($field[0]['tid']);
      //$icone_teaser = drupal_strtolower($term->name) ;
      //$icone_teaser = '' ;
      //break;
    default :
      $icone_teaser = $type;
      break;

  }

  if($icone_teaser){
    $img = array(
      'path' => drupal_get_path('theme','themeko').'/images/icones/icone-'.$icone_teaser.'.png',
      'alt' => t(str_replace('_',' ',$icone_teaser)),
      'title'=> t(ucfirst(str_replace('_',' ',$icone_teaser))),
      'attributes' => array('class' => $class)
    );

    if($with_style){
      $img['style_name'] = 'icone_content_type';
      $img['height'] = NULL;
      $img['width'] = NULL;
      return theme_image_style($img);
      //return theme('image_style',$img);
    }
    else{
      return theme('image', $img);
    }
  }
}


