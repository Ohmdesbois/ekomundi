<?php


define('SUBMIT_TILE',t('Search in title'));
define('SUBMIT_TAXO',t('Search in taxo'));
define('SUBMIT_POPU',t('Search in popu'));
define('SUBMIT_CHOICE',t('Save your choice'));

define('VOC_PONDERATION',6);

require_once drupal_get_path('module','ekomundi').'/includes/define.inc';

function ekoprop_add_form($form, &$form_state,$items_ekopoint) {
  $langcode='und';

  $fc=entity_load('field_collection_item', array($items_ekopoint));

  $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');

  $ekopoint=$fc[$items_ekopoint]->{$fc_ref_ekpt}['und'][0]['target_id'];
  $ekopoint= node_load($ekopoint);

  $host_entity = $fc[$items_ekopoint]->hostEntity();
  $node_ref=$host_entity;



  $form['description']=array('#markup'=>'<h2>'._conjug_title($ekopoint,$node_ref).'</h2>');
  $form['items_ekopoint']=array(
    '#type' => 'hidden',
    '#value' => $items_ekopoint,
  );
  $form['form_search']=array(
    '#type' => 'vertical_tabs',
  );


  //champ fieldset du formaire
  $form['noderef'] = array(
    '#type'  => 'fieldset',
    '#group' => 'form_search',
    '#title'   => t('Search by popularity'),
    '#attributes'  => array('id'=>array( 'fieldset_search_noderef')),
  );


  $propositions=field_get_items('node',$ekopoint, variable_get('ekomundi_ekopoint_field_ref_prop'));

  if(!empty($propositions)){

    $popular_props= array_count_values(array_values_recursive($propositions));

    arsort($popular_props);
    $props=array();
    foreach($popular_props as $nid => $count){
      $node=node_load($nid);
      $props[$node->nid]=l($node->title, 'node/'.$node->nid, array('attributes' => array('target'=>'_blank'))).' ('.$count.')';
    }

    $form['noderef']['res_pop'] = array(
      '#type' => 'checkboxes',
      //'#markup' => 'un texte nouveau dans taxo ',
      '#title' => t('Choose what propositions to add in your ekopoint'),
      '#options' => $props,
      '#ajax' => array(
            'callback' => 'ajax_res_choice_callback',
            'wrapper' => 'res_choice_div',
          )
    );
  }
  else{
    $form['noderef']['res_pop'] = array(
      '#markup' => t('There are not propositions referenced in this ekopoint. Try to search by title, by taxonomie or create a new one.'),
    );
  }


  //champ fieldset du formaire
  $form['titre'] = array(
    '#type'  => 'fieldset',
    '#group' => 'form_search',
    '#title'   => t('Search by title'),
    '#attributes'  => array('id'=>array( 'fieldset_search_titre')),
  );
  $form['titre']['title_prop'] = array(
    '#title' => t('Title'),
    //'#description'=> t('Type your texte then press ENTER'),
    '#type' => 'textfield',
  );
  $form['titre']['submit_titre'] = array(
    '#type' => 'button',
    '#value' => SUBMIT_TILE,
    '#ajax' => array(
      'callback' => 'ekoprop_form_titre_callback',
      'wrapper' => 'res_titre_search_div',
    )
  );
  $form['titre']['res_titre'] = array(
    '#prefix' => '<div id="res_titre_search_div">',
    '#suffix' => '</div>',
  );


  //champ fieldset du formaire
  $form['taxo'] = array(
    '#type'  => 'fieldset',
    '#group' => 'form_search',
    '#title'   => t('Search by taxonomie'),
    '#attributes'  => array('id'=>array( 'fieldset_search_taxo')),
  );

  //definit les vocabulaires autoriser
  $point_vocab =  array(variable_get('ekomundi_voc_domaine'),variable_get('ekomundi_voc_action'),variable_get('ekomundi_voc_objet'));
  //recupere tout les terme de vocabulaire
  $vocabularies = taxonomy_get_vocabularies();

  //pour tous les termes de vocabulaire
  $vocab_array = array();
  foreach ($vocabularies as $vocabulary) {
    $vid = $vocabulary->vid;
    $vmachine = $vocabulary->machine_name;
    if(in_array($vmachine,$point_vocab)){ //si le vocaublaire est autorisé
      //$vname = i18n_taxonomy_vocabulary_name($vocabulary);
      if(module_exists('i18n')){
        $vname=  i18n_string(array('taxonomy', 'vocabulary', $vid, 'name'), $vocabulary->name, array('langcode' => $langcode));
        $vocab_array[$vid] = array('vmachine'=>$vmachine,'name'=>$vname);
      }
      else{
        $vocab_array[$vid] = array('vmachine'=>$vmachine,'name'=> $vocabulary->name);
      }

    }
  }
  //pour tous les terme de vocabulaire sélectionner
  foreach($vocab_array as $vid => $voc ){
    //recupere les termes du vocabulaire
    $taxo=taxonomy_get_tree($vid);
    //creer les options pour le champ select du vocabulaire
    $term_array = array('0' => '--choisir--');
    //dpm($taxo);
    foreach ($taxo as $term) {
      $tid = $term->tid;
      //$term_array[$tid] = str_repeat('-',$term->depth).i18n_taxonomy_term_name($term,$langcode);
      $term_array[$tid] = str_repeat('-',$term->depth).$term->name;
    }
    $default_value = $ekopoint->{'field_tid_'.$voc['vmachine'].'s'}['und'][0]['tid'];
    //creation d'un champ select pour un vocab avec ses termes
    $form['taxo']['voc_'.$vid] = array(

      '#field_name' => 'voc_'.$vid,
      '#language' => 'und',
      '#field_parents' => '',
      '#columns' => array('tid'),
      '#title' => t($voc['name']),
      '#description' => '',
      '#required' => FALSE,
      '#delta' => 0,
      '#value' => '',
      '#field' => 'term_reference_tree',
      '#attributes' => array('class'=>array( 'field-widget-term-reference-tree')),
      '#attached' => array(
          'js' => array(
            drupal_get_path('module', 'entity_reference_tree').'/entity_reference_tree.js' ),
          'css' => array(
            drupal_get_path('module', 'entity_reference_tree').'/css/entity_reference_tree.css',
            drupal_get_path('module', 'entity_reference_tree').'/css/term_reference_tree.css'),
        ),
      '#type' => 'checkbox_tree',
      '#default_value' => array($default_value=>$default_value),
      '#max_choices' => -1,
      '#start_minimized' => 1,
      '#leaves_only' => 0,
      '#filter_view' => '',
      '#select_parents' => 0,
      '#track_list' => 0,
      '#allowed_ids' => '',
      '#allowed_bundles' => array('#parent_id' => 0, '#vocabulary' => $vid),
      '#token_display' => '',
      '#value_key' => 'tid',
      '#element_validate' => array('_entity_reference_tree_widget_validate'),
      '#properties' => '',
      '#after_build' => array('field_form_element_after_build'),
      '#ajax' => array(
        'callback' => 'ekoprop_form_taxo_callback',
        'wrapper' => 'res_taxo_search_div',
      )
    );
  }


  $form['taxo']['res_taxo'] = array(
    '#prefix' => '<div id="res_taxo_search_div">',
    '#suffix' => '</div>',
    //'#markup' => 'un texte dans taxo',
  );

  //champ fieldset du formaire
  $form['create'] = array(
    '#type'  => 'fieldset',
    '#group' => 'form_search',
    '#title'   => t('Create new proposition'),
    '#attributes'  => array('id'=>array( 'fieldset_create_prop')),
  );
  //champ fieldset du formaire
  $form['create']['text'] = array(
    '#markup' => t('You do not find propositions corresponding to your ekopoints. So we suggest you to create a new one.').' '. l(t('Create @name',array('@name'=>'proposition')), 'node/add/proposition',array('query' => array('field_col_ekopoint' => $items_ekopoint)) ),
  );

  $form['choice'] = array(
    '#prefix' => '<div id="res_choice_div">',
    '#suffix' => '</div>',
  );

  if(isset($form_state['values']) && $form_state['values']['op']!= SUBMIT_CHOICE){
    if($form_state['values']['form_search__active_tab']=='fieldset_search_taxo'){
      //dpm($form_state['values']);
      $query=db_select('node','n');
      $query->fields('n',array('nid','title'));
      $query->condition('n.type','proposition','=');

      //$query->join('taxonomy_index', 't', 'n.nid = t.nid');
      $count_value=0;
      foreach($form_state['values'] as $id=>$values) {
        if(drupal_substr($id,0,4)=='voc_' &&  !empty($values) &&  is_array($values) ) {
          switch ($id){
            case 'voc_1' : //object
              if(count($form_state['values']['voc_1'] )>0){
                $count_value++;
                $query->join('field_data_field_tid_objets', 'o', "n.nid = o.entity_id AND (o.entity_type = 'node' AND o.deleted = '0')");
                $in=array();
                foreach($form_state['values']['voc_1'] as $d => $v){
                  $in[] = $v['tid'];
                }
                $query->condition('o.field_tid_objets_tid',$in,'IN');
              }
              break;
            case 'voc_7' : //action
              if(count($form_state['values']['voc_7'] )>0){
                $count_value++;
                $query->join('field_data_field_tid_actions', 'a', "n.nid = a.entity_id AND (a.entity_type = 'node' AND a.deleted = '0')");
                $in=array();
                foreach($form_state['values']['voc_7'] as $d => $v){
                  $in[] = $v['tid'];
                }
                $query->condition('a.field_tid_actions_tid',$in,'IN');
              }
              break;
            case 'voc_8' : //domaine
              if(count($form_state['values']['voc_8'] )>0){
                $count_value++;
                $query->join('field_data_field_tid_domaines', 'd', "n.nid = d.entity_id AND (d.entity_type = 'node' AND d.deleted = '0')");
                $in=array();
                foreach($form_state['values']['voc_8'] as $d => $v){

                  $in[] = $v['tid'];
                }
                $query->condition('d.field_tid_domaines_tid',$in,'IN');
              }
              break;
          }

        }
      }
      //dpq($query);
      if($count_value>0){
        $result = $query->execute();

        $props=array();
        while($record = $result->fetchAssoc()) {
          $props[$record['nid']]=l($record['title'], 'node/'.$record['nid'], array('attributes' => array('target'=>'_blank')));
        }
        if(count($props)>0 ){
          $form['taxo']['res_taxo'] = array(
            '#prefix' => '<div id="res_taxo_search_div">',
            '#suffix' => '</div>',
            '#type' => 'checkboxes',
            //'#markup' => 'un texte nouveau dans taxo ',
            '#title' => t('Choose what propositions to add in your ekopoint'),
            '#options' => $props,
            '#ajax' => array(
                  'callback' => 'ajax_res_choice_callback',
                  'wrapper' => 'res_choice_div',
                )
          );
        }
        else{
          $form['taxo']['res_taxo'] = array(
            '#prefix' => '<div id="res_taxo_search_div">',
            '#suffix' => '</div>',
            //'#type' => 'checkboxes',
            '#markup' => t('Try other choices, there are no propositions'),
            '#title' => t('Match not found'),
            //'#options' => $props,
            //'#ajax' => array(
             //     'callback' => 'ajax_res_choice_callback',
             //     'wrapper' => 'res_choice_div',
             //   )
          );
        }
      }


    }
    elseif($form_state['values']['form_search__active_tab']=='fieldset_search_titre'){
      //switch($form_state['values']['op']) {
      //  case SUBMIT_TILE :

          if(!empty($form_state['values']['title_prop']) && strlen($form_state['values']['title_prop'])>=3){
            $query=db_select('node','n');
            $query->fields('n',array('nid','title'));
            $query->condition('n.type','proposition','=');

            $query->condition('n.title', '%' . db_like($form_state['values']['title_prop']) . '%', 'LIKE');


            $result = $query->execute();

            $num_of_results = $query->execute()->rowCount();
            $props=array();
            $count_res=0; $max_res=20;

            while($record = $result->fetchAssoc() ) {
              $props[$record['nid']]=l($record['title'], 'node/'.$record['nid'], array('attributes' => array('target'=>'_blank')));
              $count_res++;
              if( $count_res >= $max_res){
                  break;
              }
            }
            if($num_of_results>0){
              $form['titre']['res_titre'] = array(
                '#prefix' => '<div id="res_titre_search_div">',
                '#suffix' => '</div>',
                '#type' => 'checkboxes',
                '#description' => ($num_of_results > $max_res) ? t('@n1 results displayed on @n2.',array('@n1'=>$max_res,'@n2'=>$num_of_results)) : '',
                '#title' => t('Choose what propositions to add in your ekopoint'),
                '#options' => $props,
                '#ajax' => array(
                      'callback' => 'ajax_res_choice_callback',
                      'wrapper' => 'res_choice_div',
                    )
              );
            }
            else{
              $form['titre']['res_titre'] = array(
                '#prefix' => '<div id="res_titre_search_div">',
                '#suffix' => '</div>',
                '#title' => t('Match not found'),
                '#markup' => t('There is no result for this title.')
              );
            }


          }
          else{
            $form['titre']['res_titre'] = array(
              '#prefix' => '<div id="res_titre_search_div">',
              '#suffix' => '</div>',
              '#title' => t('Error'),
              '#markup' => t('The title is too short !')
            );
          }

        //  break;
      //}
    }

    if(!empty($form_state['values']['res_pop']) || !empty($form_state['values']['res_titre']) ||!empty($form_state['values']['res_taxo']) ){

      $global_res=array();
      $global_res= isset($form_state['values']['res_pop']) ? array_merge($global_res,$form_state['values']['res_pop']) : $global_res ;
      $global_res= isset($form_state['values']['res_titre']) ? array_merge($global_res,$form_state['values']['res_titre']) : $global_res ;
      $global_res= isset($form_state['values']['res_taxo']) ? array_merge($global_res,$form_state['values']['res_taxo']) : $global_res ;

      $count_res = 0;
      $oselected = '<ul>';$selected=array();
      foreach($global_res as $res){
        if(!empty($res)){
          $node=node_load($res);
          $oselected .= '<li>'.$node->title.'</li>';
          $selected[]=$res;
          $count_res++;
        }
      }
      $oselected .='</ul>';

      if($count_res>0){
        $form['choice'] = array(
          '#prefix' => '<div id="res_choice_div">',
          '#suffix' => '</div>',
          '#type'  => 'fieldset',
          '#title' => t('You have selected :'),
        );
        $form['choice']['res'] = array(
          '#markup' => $oselected,
        );
        $form['choice']['selected'] = array(
          '#type' => 'hidden',
          '#value' => $selected,
        );
        //recupere les termes du vocabulaire
        $name_ponderation = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_ponderation'));
        $vid_ponderation = $name_ponderation->vid;
        $taxo=taxonomy_get_tree($vid_ponderation);
        $term_array=array();
        foreach ($taxo as $term) {
          $tid = $term->tid;
          //$term_array[$tid] = str_repeat('-',$term->depth).i18n_taxonomy_term_name($term,$langcode);
          $term_array[$tid] = str_repeat('-',$term->depth).$term->name;
        }
        $form['choice']['ponderation'] = array(
          '#type' => 'select',
          '#title' => t('Weighting'),
          '#options' => $term_array,
          '#default_value' => variable_get('ekomundi_dv_ponderation'),
          '#description' => t('Set the weighting for the selected propositions.'),
        );
        $form['choice']['save_choice'] = array(
          '#type' => 'submit',
          '#value' => SUBMIT_CHOICE,

        );
      }

    }


  }
  return $form;
}

function ekoprop_form_search_callback($form, $form_state) {
  return $form['noderef']['res_pop'];
}
function ekoprop_form_taxo_callback($form, $form_state) {
  return $form['taxo']['res_taxo'];
}
function ekoprop_form_titre_callback($form, $form_state) {
  return $form['titre']['res_titre'];
}
function ajax_res_choice_callback($form, $form_state) {
  return $form['choice'];
}

function ekoprop_add_form_validate($form, &$form_state) {
  global $user;

  $fc=entity_load('field_collection_item', array( $form_state['values']['items_ekopoint'] ) );

  if(isset($fc[$form_state['values']['items_ekopoint']]) ){

    $fc=$fc[$form_state['values']['items_ekopoint']];
    $host_entity = $fc->hostEntity();

    $user_membership = og_get_membership('node',$host_entity->nid,'user',$user->uid);


    $has_access=FALSE;
    //si c'est un action regarde si membre dans les groupes parent
    $oeuvre = variable_get('ekomundi_allowed_ekopoint_types');

    if(in_array($host_entity->type, $oeuvre)){
      $group=og_get_entity_groups('node',$host_entity->nid);
      if(isset($group['node'])){
        foreach($group['node'] as $id => $etid){
          $user_membership = og_get_membership('node',$etid,'user',$user->uid);
          if($user_membership && $user_membership->state==1){
            $has_access=TRUE;
          }
        }
      }
    }
    /* TO DO : add a control acces
    $communaute = array(CT_INTENTION, CT_ACTIVITE, CT_ORGANISME);
    if((in_array($host_entity->type, $communaute) && $host_entity->uid == $user->uid) ||($user_membership && $user_membership->state==1) || $has_access){
    }
    else{
      form_set_error('items_ekopoint',t('ACCESS ERROR : This is impossible to add proposition to this ekopoint !'));
    }
    */
  }
  else{
    form_set_error('items_ekopoint',t('This is impossible to add proposition to this ekopoint !'));
  }

}


function ekoprop_add_form_submit($form, &$form_state) {


  $fc=entity_load('field_collection_item',array($form_state['values']['items_ekopoint']) );
  $fc=$fc[$form_state['values']['items_ekopoint']];
  $host_entity = $fc->hostEntity();

  $fed = variable_get('ekomundi_ekopoint_field_domaine');
  $feo = variable_get('ekomundi_ekopoint_field_objet');
  $fea = variable_get('ekomundi_ekopoint_field_action');

  //load taxonomie of the ekopoint to populate the proposition

  $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');
  $ekopoint = field_get_items('field_collection_item',$fc,$fc_ref_ekpt);
  $ekopoint = node_load($ekopoint[0]['target_id']);


  $domaines=field_get_items('node',$ekopoint,$fed);
  $actions=field_get_items('node',$ekopoint,$fea);
  $objets=field_get_items('node',$ekopoint,$feo);

  $domaines_ekopoint = array();
  foreach($domaines as $term){
    $terms = taxonomy_get_parents_all($term['tid']);
    foreach($terms as $parent){
      $domaines_ekopoint[]['tid'] = $parent->tid;
    }
  }
  $actions_ekopoint = array();
  foreach($actions as $term){
    $terms = taxonomy_get_parents_all($term['tid']);
    foreach($terms as $parent){
      $actions_ekopoint[]['tid'] = $parent->tid;
    }
  }
  $objets_ekopoint = array();
  foreach($objets as $term){
    $terms = taxonomy_get_parents_all($term['tid']);
    foreach($terms as $parent){
      $objets_ekopoint[]['tid'] = $parent->tid;
    }
  }

  //load referenced proposition to check if not exist
  $referenced_prop=array();
  $fc_ekpp = variable_get('ekomundi_field_col_ekoprop');
  $fc_ref_ekpp = variable_get('ekomundi_fc_ref_ekoprop');

  if(isset($fc->{$fc_ekpp}['und'])){
    foreach($fc->{$fc_ekpp}['und'] as $delta => $items_prop){
      $fc_prop=entity_load('field_collection_item',array($items_prop['value']) );
      $fc_prop=$fc_prop[$items_prop['value']];

      $ekoprop = field_get_items('field_collection_item',$fc_prop,$fc_ref_ekpp);
      $referenced_prop[]=$ekoprop[0]['target_id'];
    }
  }

  $count=0;
  foreach($form_state['values']['selected'] as $res){
    if(!empty($res)){

      $already_in_ekopoint=FALSE;
      if(in_array($res,$referenced_prop)){
         $already_in_ekopoint=TRUE;
      }

      if(!$already_in_ekopoint){
        $fpd = variable_get('ekomundi_prop_field_domaine');
        $fpo = variable_get('ekomundi_prop_field_objet');
        $fpa = variable_get('ekomundi_prop_field_action');

        // Create and save field collection for node
        $field_collection_item = entity_create('field_collection_item', array('field_name' => $fc_ekpp));
        $field_collection_item->setHostEntity('field_collection_item', $fc);
        $field_collection_item->{$fc_ref_ekpp}[LANGUAGE_NONE][]['target_id'] = $res;
        $field_collection_item->field_tid_ponderation_1[LANGUAGE_NONE][0]['tid'] = $form_state['values']['ponderation'];
        $field_collection_item->save();

        //populate the proposition with the taxonomy of the ekopoint
        $proposition = node_load($res);
        $domaines_prop = field_get_items('node',$proposition,$fpd);
        $actions_prop = field_get_items('node',$proposition,$fpa);
        $objets_prop = field_get_items('node',$proposition,$fpo);

        $proposition->{$fpd}['und'] = merge_fields_values($domaines_ekopoint,$domaines_prop,'tid');
        $proposition->{$fpa}['und'] = merge_fields_values($actions_ekopoint,$actions_prop,'tid');
        $proposition->{$fpo}['und'] = merge_fields_values($objets_ekopoint,$objets_prop,'tid');

        node_save($proposition);
        unset($proposition);

        $count++;
      }
      else{
        $ekoprop=node_load($res);
        drupal_set_message( t('<em>@title</em> already exists in this ekopoint.',array('@title'=>$ekoprop->title)),'warning');
      }
    }
  }
  if($count>0){
    $ocount=format_plural($count, '1 proposition', '@count propositions');
    $form_state['redirect'] = 'node/' . $host_entity->nid;
    drupal_set_message(t('@nb has been added to the ekopoint.', array('@nb'=>$ocount)));

  }
  else{
    $ocount=t('No proposition');
    drupal_set_message(t('@nb has been added to the ekopoint.', array('@nb'=>$ocount)));
  }

}
