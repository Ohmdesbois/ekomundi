<?php


class og_hierarchy_handler_relationship extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();
    //dpm(array($this,$table_data,$base_field));
    $def = $this->definition;
    $def['table'] = 'og_membership';//$this->definition['base'];
    $def['table formula'] = 'og_membership';
    $def['field'] = 'gid';
    $def['left_table'] = 'og_membership_og_membership';//$this->table_alias;
    $def['left_field'] = 'etid';
    if (!empty($this->options['required'])) {
      $def['type'] = 'INNER';
    }

    if (!empty($this->definition['extra'])) {
      $def['extra'] = $this->definition['extra'];
    }

    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler'];
    }
    else {
      $join = new views_join();
    }
    dpm($def);
    $join->definition = $def;
    $join->options = $this->options;
    $join->construct();
    $join->adjusted = TRUE;
    dpm($join);
    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);

    // Add access tags if the base table provide it.
    if (empty($this->query->options['disable_sql_rewrite']) && isset($table_data['table']['base']['access query tag'])) {
      $access_tag = $table_data['table']['base']['access query tag'];
      $this->query->add_tag($access_tag);
    }
  }

  /**
   * You can't groupby a relationship.
   */
  function use_group_by() {
    return FALSE;
  }
}
