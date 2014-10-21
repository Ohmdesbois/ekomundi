<?php


include_once drupal_get_path('module','ekomundi').'/ekomundi.concat.inc';
/*
	Implement : Hook_form
	creer un formulaire de choix de term par vocabulaires
	affiche toutes les phrase générer
*/

function ekopoint_add_form($form, &$form_state,$add_to_nid) {
	global $language ;
	global $user;
	//drupal_add_js(drupal_get_path('module','eko_concat').'/eko_concat.js',array('weight'=>100));
	//$langcode = ($language->language=='fr')? 'und' : $language->language  ;
	$langcode =  $language->language ;

  $user=user_load($user->uid);
	//dpr	($user);

	//$current_project=$user->field_current_project['und'][0]['value'];


	//$is_in_current_project=array();  // pour stocker les valeur initialiser dans le projet
	//si current projet est renseigné alors on charge le projet

	//$node=node_load($current_project);

  //definit les vocabulaires autoriser
  $name_domaine = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_domaine'));
  $vid_domaine = $name_domaine->vid;
  $name_action = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_action'));
  $vid_action = $name_action->vid;
  $name_objet = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_objet'));
  $vid_objet = $name_objet->vid;
  $name_etape = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_etape'));
  $vid_etape = $name_etape->vid;
  $name_niveau = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_niveau'));
  $vid_niveau = $name_niveau->vid;



  $point_vocab = array(variable_get('ekomundi_voc_domaine'),variable_get('ekomundi_voc_action'),variable_get('ekomundi_voc_objet'));

	//recupere tout les terme de vocabulaire
	$vocabularies = taxonomy_get_vocabularies();

	//pour tous les termes de vocabulaire
 	$vocab_array = array();
  //$form['#validate'][]='eko_jo';
  foreach ($vocabularies as $vocabulary) {
		$vid = $vocabulary->vid;
    $vmachine = $vocabulary->machine_name;
		if(in_array($vmachine,$point_vocab)){ //si le vocaublaire est autorisé

      if(module_exists('i18n')){
        $vname=  i18n_string(array('taxonomy', 'vocabulary', $vid, 'name'), $vocabulary->name, array('langcode' => $langcode));
        $vocab_array[$vid] = array('vmachine'=>$vmachine,'name'=>$vname);
      }
      else{
        $vocab_array[$vid]= array('vmachine'=>$vmachine,'name'=> $vocabulary->name);
      }
		}
  }

	//champ fieldset du formaire
	$form['thefield'] = array(
		'#type'  => 'fieldset',
		'#title'   => t('You can leave 1 empty field at max !'),

	);

	//pour tous les terme de vocabulaire sélectionner
	foreach($vocab_array as $vid => $voc ){
		//recupere les termes du vocabulaire
		$taxo=taxonomy_get_tree($vid);
		//creer les options pour le champ select du vocabulaire
		$term_array=array('0' => '--choisir--');
		foreach ($taxo as $term) {

			$tid = $term->tid;
			//$term_array[$tid] = str_repeat('-',$term->depth).i18n_taxonomy_term_name($term,$langcode);
      $term_array[$tid] = str_repeat('-',$term->depth).$term->name;
		}

		//si le vocabulaire n'est pas initialisé dans le projet
		if(!isset($node->{'field_'.$voc['vmachine']}['und'][0]['tid'])){

      //creation d'un champ select pour un vocab avec ses termes
      $form['thefield']['voc_'.$vid] = array(

        '#title'           => t($voc['name']),
        '#type'            => 'checkbox_tree',
        '#language'        => $langcode,
        '#value_key'       => 'tid',
        '#field_name'      => 'voc_'.$vid,

        '#field'           => 'term_reference_tree',

        '#attributes'      => array('class'=>array( 'field-widget-term-reference-tree')),
        '#required'        => FALSE,
        '#token_display'   => '',
        '#leaves_only'     => 0,
        '#select_parents'  => 0,
        '#track_list'      => 0,
        '#delta'           => 0,
        '#max_choices'     => -1,
        '#max_depth'       => '',
        '#start_minimized' => 1,
        '#description' => '',
        '#value' => '',
        '#properties' => '',
        '#allowed_bundles' => array('#parent_id' => 0, '#vocabulary' => $vid),
        //'#vocabulary' => $vid,
        '#allowed_ids'     => '',
        '#field_parents'   => '',
        '#filter_view'     => '',
        '#columns'         => array('tid'),
        '#element_validate' => array('_entity_reference_tree_widget_validate'),
        '#after_build'      => array('field_form_element_after_build'),

        '#default_value'    => '',
          '#attached' => array(
              'js' => array(
                drupal_get_path('module', 'entity_reference_tree').'/entity_reference_tree.js' ),
              'css' => array(
                drupal_get_path('module', 'entity_reference_tree').'/css/entity_reference_tree.css',
                drupal_get_path('module', 'entity_reference_tree').'/css/term_reference_tree.css'),
            ),
        '#ajax' => array(
          'callback' => 'ajax_res_concat_callback',
          'wrapper' => 'res_concat_div',
        )

      );


		}
		else{
			// si les terme du voc initialiser dans le projet sont multiple
			if(sizeof($node->{'field_'.$voc['vmachine']}['und'])>1){
				$d=array();
				$d2=array();
				foreach($node->{'field_'.$voc['vmachine']}['und'] as $delta){
					$d[]=$delta['tid'];

					//$d2[$delta['tid']] = i18n_taxonomy_term_name(taxonomy_term_load($delta['tid']),$langcode);
          $term_2=taxonomy_term_load($delta['tid']);
          $d2[$delta['tid']] = $term_2->name;
				}
				// si le voc n'est pas un domaine
				if($voc['vmachine'] != variable_get('ekomundi_voc_domaine')){
					$is_in_current_project[$vid] = $d;
					$form['thefield']['voc_'.$vid] = array(
						'#type'             => 'hidden',
						'#default_value'    => json_encode($d),
					);
				}
				else{

				  $form['thefield']['voc_'.$vid] = array(
						'#type'             => 'checkboxes',
						'#title'            => t($voc['name']),
						'#title_display'    => 'before',
						'#position'         => 'left' ,
						'#options'          => $d2 ,
						'#default_value'    => $d,
						'#ajax' => array(
							'callback' => 'ajax_res_concat_callback',
							'wrapper' => 'res_s_div',
						)
					);
				}
			}
			else{
				//creation
				if($voc['vmachine'] != variable_get('ekomundi_voc_domaine')){
					$is_in_current_project[$vid] = $node->{'field_'.$voc['vmachine']}['und'][0]['tid'];
				}
				$form['thefield']['voc_'.$vid] = array(
					'#type'             => 'hidden',
					'#default_value'    => isset($form_state['storage']['voc'][$vid]) ? $form_state['storage']['voc'][$vid] : $node->{'field_'.$voc['vmachine']}['und'][0]['tid'],
				);
			}
		}
	}

	  // This entire form element will be replaced whenever 'changethis' is updated.
  $form['res_concat'] = array(
    '#prefix' => '<div id="res_concat_div">',
    '#suffix' => '</div>',
  );

  $form['res_choice'] = array(
    '#prefix' => '<div id="res_submit_div">',
    '#suffix' => '</div>',
  );



	$nb_max = 1;  //nombre max de champ vide
	$count = 0;
	//si le formulaire a été envoyé via ajax

	if(isset($form_state['values'])){

		//compte le monbre de champ non selectioner
		foreach($form_state['values'] as $id=>$value) {
			if(drupal_substr($id,0,4) == 'voc_' && empty($value[0]['tid'])) {
        $count++;
      }
		}
		//si le compte est inferieur au nombre max
		if($count <= $nb_max){
			//$lang_name = ($language->language=='fr')? 'und' : $language->language  ;
			//$lang_name =  $language->language ;
			$lang_name='und';

      $fe = variable_get('ekomundi_field_etape');

			$node_ref = node_load($add_to_nid);
      $etape = $node_ref->{$fe}[$lang_name][0]['tid'];
      $type = $node_ref->type;
			$add_to_pid=array(); // pour stocker la liste des termes non séléctionner



			$pids[0]=array(); //
			foreach($form_state['values'] as $vid=>$tid) {

        if( drupal_substr($vid,0,4)=='voc_') 	{
					list(,$vid) = explode('_',$vid);
					//$tid=(json_decode($tid[0])? json_decode($tid[0]) : $tid[0]['tid'] );

          //si le terme est a zero alors on recupere touts les termes du vocabulaire
					if(empty($tid) && !is_array($tid)){
						$taxo=  taxonomy_get_tree($vid);
						foreach ($taxo as $item) {
							$key = $item->tid;
							$add_to_pid[$vid][] = $item->tid;
						}
					}
					//si le terme est multiple alors on ajoute les valeur pour la generation des point
					elseif(is_array($tid)){

						$all_tids = array();
						$array_is_null = true;

						foreach($tid as $k=>$t){
							$all_tids[]=$k;
							if(!empty($t['tid'])){ $array_is_null = false; $add_to_pid[$vid][] = $t['tid'];}
						}

						if($array_is_null){
              $taxo = taxonomy_get_tree($vid);
              foreach ($taxo as $item) {
                $key = $item->tid;
                $add_to_pid[$vid][] = $item->tid;
              }
            }
					}

          else{
						$pids[0][$vid] = $tid;
					}
				}
			}
			$vid_to_sort = $vid_domaine;
			//generer un tableau de tous les point a concatener
      //dpm(array($pids,$add_to_pid));
			$pids = get_all_pids_if_some_term_is_null($pids,$add_to_pid);

			require_once DRUPAL_ROOT.'/sites/all/modules/ekomundi/includes/array-sorter.inc';
			$sorter = new array_sorter($pids, $vid_to_sort);
			$pids = $sorter->sortit();


			$output = '';
			$options = array();

			//pour tous les point a concatener
			for($i = 0;$i<sizeof($pids);$i++){
				$pids[$i][$vid_etape] = $etape;
        $pids[$i][$vid_niveau] = 'singulier';
        switch($type){
            case 'vision': $genre = 'pluriel2';
              break;
            case 'projet': $genre = 'pluriel1';
              break;
            case 'intention': $genre = 'singulier';
              break;
        }
        //$pids[$i][$vid_niveau] = 'singulier';
				//creer le pid du point concatener
				$pid_to_bd='';
				foreach($pids[$i] as $v=>$t){
					//if(!in_array($t,$is_in_current_project)){
						$pid_to_bd.=$v.'-'.$t.'_';
					//}

				}

				$pid_to_bd=drupal_substr($pid_to_bd,0,-1);
				//tableau avec le pid du point et sa concatenation correspondante
				$add_br = ($i<(sizeof($pids)-1) && $pids[$i][$vid_to_sort]!=$pids[$i+1][$vid_to_sort] ) ? '<br/><br/>' : '' ;
				//dpm($pids[$i]);
        $options['ekopoint_'.$pid_to_bd]=ekomundi_concat($pids[$i]).$add_br;
			}

      $form['res_concat'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="res_concat_div">',
        '#suffix' => '</div>',
      );
			//liste de checkbox pid => phrase
			$form['res_concat']['pids'] = array(
				'#type' => 'checkboxes',
				'#title' => t('Choose what ekopoint to add in your project'),
				'#options' => $options,
        '#ajax' => array(
							'callback' => 'ajax_res_submit_callback',
							'wrapper' => 'res_submit_div',
						)
			);
      //bouton de validation du formulaire
      $form['res_concat']['submit_concat_form'] = array(
        '#prefix' => '<div id="res_submit_div">',
        '#suffix' => '</div>',
      );


      $nb_ekopoint=0;

      //compte le monbre de champ non selectioner
      foreach($form_state['values']['pids'] as $id=>$value) {

        if(drupal_substr($id,0,9)=='ekopoint_' && $value !='0') {
          $nb_ekopoint++;
        }
      }

      //si le compte est inferieur au nombre max
      if($nb_ekopoint >= 1){
        if(!empty($add_to_nid) && is_numeric($add_to_nid)){
          $form['res_choice']['projectid'] = array(
            '#type' => 'hidden',
            '#value' => $add_to_nid,
          );
          //bouton de validation du formulaire
          $form['res_choice']['submit_concat_form'] = array(
            '#prefix' => '<div id="res_submit_div">',
            '#suffix' => '</div>',
            '#type' => 'submit',
            '#value' => t('Add to my project'),
          );
        }
        else{
          $query=db_select('node','n')
          ->fields('n',array('nid','title'))
          ->condition('n.uid',$user->uid,'=')
          ->condition('n.type',variable_get('ekomundi_parent_ekopoint_types'),'IN');
          $result = $query->execute();

          $projects=array();
          $nres=0;
          while($record = $result->fetchAssoc()) {
            $nres++;
            $projects[$record['nid']]=$record['title'];
          }

          $form['res_choice'] = array(
            '#type' => 'fieldset',
            '#prefix' => '<div id="res_choice_div">',
            '#suffix' => '</div>',
          );
          //liste de checkbox pid => phrase
          $form['res_choice']['projectid'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Choose in witch group'),
            '#options' => $projects,
          );

          if($nres>0){
              //bouton de validation du formulaire
            $form['res_choice']['submit_concat_form'] = array(
              '#prefix' => '<div id="res_submit_div">',
              '#suffix' => '</div>',
              '#type' => 'submit',
              '#value' => t('Add to my group'),
            );
          } // end of  if($nres>0){
        }// end of
      }// end of  if($nb_ekopoint<=1){
		}  // end of */
	}// end of  if(isset($form_state['values']


  return $form;
}

function ajax_res_concat_callback($form, $form_state) {
	return $form['res_concat'];
}

function ajax_res_choice_callback($form, $form_state) {
	return $form['res_choice'];
}
function ajax_res_submit_callback($form, $form_state) {
	return $form['res_choice'];
}

function ekopoint_add_form_validate($form, &$form_state) {
	/*
	form_set_error('thefield', t($m));
  */
}

function ekopoint_add_form_submit($form, &$form_state) {
	global $language ;
	global $user;
	$langcode =  $language->language ;

  $user=user_load($user->uid);

  //dpm($form_state['values']['pids']);

  foreach($form_state['values']['pids'] as $k => $v){
    if($v!='0'){
      $pid=array();
      $datas=explode('_',$v);
      foreach($datas as $term){
        if($term != 'ekopoint'){
          list($vid,$tid)=explode('-',$term);
          $pid+=array($vid => $tid);

        }
      }
      $query=db_select('taxonomy_index','t');
      $query->join('node', 'n', 't.nid = n.nid');
      $query->fields('t',array('nid'));
      $query->condition('n.type',variable_get('ekomundi_ekopoint_type'),'=');
      /*$query->condition(db_or()
      ->condition('t.tid',$pid[VID_OBJET],'=')
      ->condition('t.tid',$pid[VID_ACTION],'=')
      ->condition('t.tid',$pid[VID_DOMAINE],'='));
      */
      $query->condition('t.tid',$pid,'IN');

      $result = $query->execute();

      // check_nb_taxo combien de fois le noeud a un term de taxo correspondant
      $check_nb_taxo=array();
      while($records= $result->fetchAssoc()){

        if( isset($check_nb_taxo[$records['nid'] ] ) ){
          $check_nb_taxo[$records['nid']]=$check_nb_taxo[$records['nid']]+1;
        }
        else{
          $check_nb_taxo[$records['nid']]=1;
        }
      }
      $ekopoint_find=array_search('3',$check_nb_taxo);
      $fc_ekpt = variable_get('ekomundi_field_col_ekopoint');

      if($ekopoint_find!==FALSE){
        if(!is_array($form_state['values']['projectid'])){
          $form_state['values']['projectid']=array($form_state['values']['projectid']);
        }
        foreach($form_state['values']['projectid'] as $p => $pv){
          if($pv!='0'){
            $node_project=node_load($pv);
            $ekopoint=node_load($ekopoint_find);
            $already_in_project=FALSE;

            $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');
            if( isset($node_project->{$fc_ekpt}['und'])){
              foreach($node_project->{$fc_ekpt}['und'] as $item_id){
                $fc=entity_load('field_collection_item', array($item_id['value']));
                $fc=$fc[$item_id['value']];

                if(isset($fc->{$fc_ref_ekpt}['und']) && $fc->{$fc_ref_ekpt}['und'][0]['nid']==$ekopoint_find ){
                   $already_in_project=TRUE;
                }
              }
            }
            //dpm($already_in_project);

            if(!$already_in_project){

              // Create and save field collection for node
              $field_collection_item = entity_create('field_collection_item', array('field_name' => $fc_ekpt));
              $field_collection_item->setHostEntity('node', $node_project);
              $field_collection_item->{$fc_ref_ekpt}[LANGUAGE_NONE][]['target_id'] = $ekopoint_find;
              //$field_collection_item->field_pid_advanc[LANGUAGE_NONE][]['tid'] = 103;
              $field_collection_item->save();
              unset($field_collection_item);
              drupal_set_message(t('<i>@title</i> was added to <em>@title_project</em>.',array('@title'=>$ekopoint->title,'@title_project'=>$node_project->title) ));
            }
            else{

              drupal_set_message( t('<em>@title</em> already exists in <em>@title_project</em>.',array('@title'=>$ekopoint->title,'@title_project'=>$node_project->title)),'warning');
            }


          }
        }

        $form_state['redirect']  = 'node/'.$form_state['values']['projectid'][0];

      }
      else{

        $fed = variable_get('ekomundi_ekopoint_field_domaine');
        $feo = variable_get('ekomundi_ekopoint_field_objet');
        $fea = variable_get('ekomundi_ekopoint_field_action');

        $new_node = new StdClass();
        $new_node->type = 'ekopoint';
        $new_node->{$fea}['und'][0]['tid']=$pid[$vid_action];
        $new_node->{$fed}['und'][0]['tid']=$pid[$vid_domaine];
        $new_node->{$feo}['und'][0]['tid']=$pid[$vid_objet];

        unset($pid[$vid_niveau]);
        unset($pid[$vid_etape]);
        $ekopoint_title=ekomundi_concat($pid);
        $new_node->title= $ekopoint_title;
        $new_node->uid=$user->uid;
        $new_node->language='fr';
        node_save($new_node);
        $nid_ekopoint=$new_node->nid;
        unset($new_node);

        $i=0;
        if(!is_array($form_state['values']['projectid'])){
          $form_state['values']['projectid']=array($form_state['values']['projectid']);
        }
        foreach($form_state['values']['projectid'] as $p => $pv){
          if($pv!='0'){
            $node_project=node_load($pv);
            $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');
            // Create and save field collection for node
            $field_collection_item = entity_create('field_collection_item', array('field_name' => $fc_ekpt));
            $field_collection_item->setHostEntity('node', $node_project);
            $field_collection_item->{$fc_ref_ekpt}[LANGUAGE_NONE][]['target_id'] = $nid_ekopoint;
            $field_collection_item->field_pid_advanc[LANGUAGE_NONE][]['tid'] = 103;
            $field_collection_item->save();
            unset($field_collection_item);

            //$new_node->field_node_ref['und'][$i++]['nid']=$pv;
          }
        }
        $form_state['redirect']  = 'node/'.$form_state['values']['projectid'][0];
        drupal_set_message(t('<em>@title_ekopoint</em> was added in <em>@title_project</em>.',array('@title_ekopoint'=>$ekopoint_title,'@title_project'=>$node_project->title)));

      }
    }
  }
}

