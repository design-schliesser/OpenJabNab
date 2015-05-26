<?php
if(!file_exists("include/common.php"))
	header('Location: install.php');
require_once "include/common.php";

if(isset($_GET['logout'])) {
	unset($_SESSION['bunny']);
	unset($_SESSION['bunny_name']);
	unset($_SESSION['ztamp']);
	unset($_SESSION['ztamp_name']);
	unset($_SESSION['login']);
	$ojnAPI->SetToken('');
	unset($_SESSION['token']);
	header("Location: index.php");
}
if(!empty($_POST['login']) && !empty($_POST['password'])) {
	$r = $ojnAPI->loginAccount($_POST['login'], $_POST['password']);
	if(!strpos($r,"AD_")) {
		$_SESSION['login'] = $_POST['login'];
		$ojnAPI->setToken($r);
	} else {
		$_SESSION['message']['error'] = "Bad authentification.";
	}
	session_write_close();
	header("Location: bunny.php");
}
?>
<?php
if(isset($_SESSION['message']) && empty($_GET)) {
	if(isset($_SESSION['message']['ok'])) { ?>
	<div class="ok_msg">
	<?php	echo $_SESSION['message']['ok'];
	} else { ?>
	<div class="error_msg">
	<?php	echo $_SESSION['message']['error'];
	}
	if(empty($_GET))
		unset($_SESSION['message']);
	echo "</div>";
}
?>
<div id="home">
  <section id="homeWelcome">
    <h1>Home</h1>
    <p>Welcome to the OpenJabNab configuration page. From here you can enable or disable, configure and schedule the features that manage your Nabaztag bunny.</p>
  </section>

  <section id="homeLogin">
<?php
if(isset($_SESSION['token'])) {
?>
    <h1>Logout</h1>
    <p>Click on the link to logout: <a href="index.php?logout">Logout</a></p>
<?php
} else {
?>
	<table>
  <form method="post">
	  <tr>    
    	<td><input type="text" name="login" placeholder="Email"></td>
	  	<td><input type="password" name="password" placeholder="Password"></td>
    </tr>
    <tr>
    	<td>&nbsp;</td>
	  	<td><input class="login" type="submit" value="Login"></td>
    </tr>
	</form>
  </table>
<?php
}
?>
  </section>

  <section id="homeNew">
    <h1>New account</h1>
    <p>If you want to register your rabbit to this server, you need to create an account. <a href="register.php">Register here</a>.</p>
  </section>
  
  <div class="slidelft">
		<a href="#" id="arrowlft" class="arrowlft"></a>
  </div>
  <div class="slidert">
      <a href="#" id="arrowrt" class="arrowrt"></a>
  </div>
</div>
<?php
require_once("include/append.php");
?>
