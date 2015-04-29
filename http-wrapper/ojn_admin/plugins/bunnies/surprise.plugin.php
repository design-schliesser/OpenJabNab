<?php
if(!empty($_POST['frequency'])) {
	$retour = $ojnAPI->getApiString("bunny/".$_SESSION['bunny']."/surprise/setFrequency?value=".$_POST['frequency']."&".$ojnAPI->getToken());
	$_SESSION['message'] = isset($retour['ok']) ? $retour['ok'] : "Error : ".$retour['error'];
	header("Location: bunny_plugin.php?p=surprise");
}
if(!empty($_POST['folder'])) {
	$retour = $ojnAPI->getApiString("bunny/".$_SESSION['bunny']."/surprise/setFolder?name=".$_POST['folder']."&".$ojnAPI->getToken());
	$_SESSION['message'] = isset($retour['ok']) ? $retour['ok'] : "Error : ".$retour['error'];
	header("Location: bunny_plugin.php?p=surprise");
}
$folders = $ojnAPI->getApiList("bunny/".$_SESSION['bunny']."/surprise/getFolderList?".$ojnAPI->getToken());
$frequency = $ojnAPI->getApiString("bunny/".$_SESSION['bunny']."/surprise/getFrequency?".$ojnAPI->getToken());
$frequency = isset($frequency['ok']) ? $frequency['ok'] : '';
?>
<form method="post">
<fieldset>
<legend>Your rabbit likes surprises ...</legend>
<label>Surprise every <select name="frequency">
	<option value="1500" <?php if ($frequency==1500) echo 'selected'; ?>>5</option>
    <option value="750" <?php if ($frequency==750) echo 'selected'; ?>>10</option>
    <option value="500" <?php if ($frequency==500) echo 'selected'; ?>>15</option>
    <option value="250" <?php if ($frequency==250) echo 'selected'; ?>>30</option>
    <option value="167" <?php if ($frequency==167) echo 'selected'; ?>>45</option>
    <option value="125" <?php if ($frequency==125) echo 'selected'; ?>>60</option>
    <option value="84" <?php if ($frequency==84) echo 'selected'; ?>>90</option>
    <option value="63" <?php if ($frequency==63) echo 'selected'; ?>>120</option>
    <option value="50" <?php if ($frequency==50) echo 'selected'; ?> >150</option>
</select> minutes.</label>
<input type="submit" value="Submit">
</fieldset>
</form>
<form method="post">
<fieldset>
<legend>Voices for surprises</legend>
<select name="folder">
<?php foreach($folders as $folder) { ?>
<option value="<?php echo $folder ?>"><?php echo $folder ?></option>
<?php } ?>
</select>
<input type="submit" value="Submit">
</fieldset>
</form>
