This module allows you to restrict access to a comment by changing the template for a private comment

INSTALLATION
Normally install the module in sites/all/modules, and enable it,
then go to admin/structure/types/manage/YOUR_CONTENT_TYPE/comment/fields 
and add a boolean field with the machine name field_private_comment. 
Set it with 
On value : Private comment
Off value : Public comment

Configure permission in admin/people/permissions#module-private_comment

COMPATIBILITY 
This module works with og-7.x-2.x. The module permission will be override by og module :
a private comment can be acessible only to group members.
