<?php
if(!empty($_POST['text']) && !empty($_POST['voice'])) {
	$retour = $ojnAPI->getApiString("bunny/".$_SESSION['bunny']."/tts/say?text=".urlencode($_POST['text'])."&voice=".urlencode($_POST['voice'])."&".$ojnAPI->getToken());
	$_SESSION['message'] = isset($retour['ok']) ? $retour['ok'] : "Error : ".$retour['error'];
	header("Location: bunny_plugin.php?p=tts");
}
?>
<form method="post">
<fieldset>
<legend>Schicke einen Text</legend>
<input type="text" name="text" value="">
<select name="voice">
  <option value="Anna" selected>Anna</option>
  <option value="Markus">Markus</option>
  <option value="Petra">Petra</option>
  <option value="Yannick">Yannick</option>  
</select>
<input type="submit" value="Senden">
</fieldset>
</form>
