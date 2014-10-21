<?php

/**
 * @file
 * Provide views data and handlers for og_hierarchy.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function og_hierarchy_views_data() {
 // OG membership's related group.
/*
   $data['og_hierarchy']['table']['join'] = array(
    'og_membership' => array(
      'handler' => 'views_handler_join_og_hierarchy_children',
      'arguments' => array(
        'table' => 'og_membership',
        'left_table' => 'og_membership',
        'left_field' => 'toto',
        'field' => 'titi',
      ),
    ),
  );
  $data['og_membership']['og_hierarchy'] = array(
    'group' => t('OG hierarchy'),
    'title' => t('Get the children'),
    'help' => t('The chidren\'s node that is associated with the OG membership.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',//'og_hierarchy_handler_relationship',
      'label' => t('Children'),
      'base' => 'og_membership',
      'base field' => 'etid',
      'relationship field' => 'gid',
    ),
  );
  // ----------------------------------------------------------------------
  // og_hierarchy table
/*
  $data['og_hierarchy']['table']['group']  = t('OG membership');

  $data['og_hierarchy']['table']['join'] = array(
    'og_hierarchy' => array(
      // links to self through left.parent = right.tid (going down in depth)
      'left_field' => 'nid',
      'field' => 'parent',
    ),
  );


  $data['og_hierarchy']['parent'] = array(
    'title' => t('Parent node'),
    'help' => t('The parent node of the node. This can produce duplicate entries if you allow multiple parents.'),
    'relationship' => array(
      'base' => 'og_hierarchy',
      'field' => 'parent',
      'label' => t('Parent'),
    ),
  );

  return $data;*/

}

/**
 * Implements hook_views_data_alter().
 */
function og_hierarchy_views_data_alter(&$data) {
  //dpm($data['og_membership']);

  $data['og_membership']['gid']['argument']['handler'] = 'og_hierarchy_handler_argument_gid';
  $data['og_membership']['gid_title'] = array(
    'title' => t('Group Title'), // The item it appears as on the UI,
    'help' => t('Title of the og membership "gid" property if node.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'og_hierarchy_handler_field_gid_title',
    ),
  );


  /*
  $data['og_hiearchy']['table']['group'] = t('OG hiearchy');

  /*  // OG membership's related group.
    $data['og_hiearchy']['og_hiearchy'] = array(
      'group' => t('OG membership'),
      'title' => t('Get children from OG membership' ),
      'help' => t('Get children from OG membership.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'label' => t('Childen'),
        'base' => 'og_membership',
        'base field' => 'etid',
        'relationship field' => 'gid',
      ),
    );
    */
  // Explain to every entity how to join with draggableviews structure table.
  /*$data['og_hiearchy']['table']['join']['og_membership'] = array(
    'handler' => 'views_handler_join_og_hierarchy_children',
    'left_table' => 'og_membership', // Because this is a direct link it could be left out.
    'left_field' => 'etid',
    'field' => 'entity_id',
  );*/
  //dpm();
}

/**
 * Implements hook_views_handlers().
 * /
function og_hierarchy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'og_hierarchy') . '/views',
    ),
    'handlers' => array(
      'views_handler_join_og_hierarchy_children' => array(
        'parent' => 'views_join',
      ),
    ),
  );
}


/**
 * /

class views_handler_join_og_hierarchy_children extends views_join {
  //dpm('views_handler_join_og_hierarchy_children');
  function construct($table, $left_table, $left_field, $field, $extra = array(), $type = 'LEFT') {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
    dpm('yop');
  }
  function build_join($select_query, $table, $view_query) {
    /*$left =  $view_query->get_table_info($this->left_table);
    $right_table = "{" . 'jojo' . "}";

    $on = array();
    $on[] = "$left[alias].etid = og_membership.gid";

    $select_query->addJoin($this->type, $right_table, $table['alias'], '(((' . $on . ')))', array());
    * /
    $this->extra = 'foo.bar = baz.boing';
    parent::build_join($select_query, $table, $view_query);
  }

}
*/
