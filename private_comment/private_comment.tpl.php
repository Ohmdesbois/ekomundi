<?php

/**
 * @file
 * See comment.tpl.php for more informations about available variables.
 *
 * @see private_comment_preprocess_private_comment()
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $picture ?> 

  <?php if ($new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print $comment_is_private ?>
  
    <?php if ($signature): ?>
      <div class="user-signature clearfix">
        <?php print $signature ?>
      </div>
    <?php endif; ?>
  </div>
  <?php print render($content['links']) ?>
</div>
