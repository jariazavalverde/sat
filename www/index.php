<!DOCTYPE html>
<html lang="en">
	<head>
		<title>sat@jariaza</title>
		<meta name="description" content="Boolean satisfiability problem" />
		<meta name="author" content="José Antonio Riaza Valverde" />
		<meta charset="UTF-8" />
		<link href="https://fonts.googleapis.com/css?family=Merriweather:400,700" rel="stylesheet">
		<link href="https://fonts.googleapis.com/css?family=Open+Sans:400,700" rel="stylesheet">
		<link rel="stylesheet" href="sat.css" type="text/css" media="ALL" />
		<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.13/css/all.css" integrity="sha384-DNOHZ68U8hZfKXOrtjWvjxusGo9WQnrNx2sqG0tfsghAvtVlRW3tvkXWZh58N9jp" crossorigin="anonymous">
	</head>
	<body>
		<header>
			<h1>Boolean satisfiability problem</h1>
		</header>
		<div class="container">
			<header class="container-header">
				<h2 id="input"><a href="#input"><i class="fas fa-hashtag"></i> Input</a></h2>
			</header>
			<form action="./#output" method="POST">
				<div class="columns">
					<div class="column-left"><div>
						<textarea id="problem" name="problem" title="SAT program" placeholder="Your SAT program here..."><?php
if(isset($_GET["sample"]) && strrpos($_GET["sample"], "/") == false && file_exists("sample/" . $_GET["sample"]))
	echo file_get_contents("sample/" . $_GET["sample"]);
else if(isset($_POST["problem"]))
	echo $_POST["problem"];
?></textarea>
						<button class="button" type="submit" title="Solve it!">
							<i class="fas fa-cogs"></i> &nbsp; Solve it!
						</button>
					</div></div>
					<div class="column-right"><div>
						<h3><i class="fas fa-list-alt"></i> Options</h3>
						<div class="option"><input type="checkbox" name="st" id="st" <?php if(isset($_POST["st"])) echo "checked=\"checked\""; ?> /> <label for="st"><i class="fas fa-clock"></i> Display statistics <span class="legend">(decisions, propagations, execution time, ...)</span></label></div>
						<br />
						<h3><i class="fas fa-flask"></i> Sample problems</h3>
							<ul class="samples-list">
<?php
foreach(scandir("sample") as $f)
	if($f != "." && $f != "..")
		echo "\t\t\t\t\t\t\t\t<li><i class=\"fas fa-file\"></i> <a href=\"./?sample=$f\"><span>$f</span></a></li>\n";
?>
							</ul>
					</div></div>
				</div>
			</form>
<?php if(isset($_POST["problem"])) { ?>
			<br />
			<header class="container-header">
				<h2 id="output"><a href="#output"><i class="fas fa-hashtag"></i> Output</a></h2>
			</header>
			<div id="result">
<?php
$options = "";
if(isset($_POST["st"])) $options .= " -st";
$f = fopen("sat.cnf", "w");
fwrite($f, $_POST["problem"]);
fclose($f);
$result = shell_exec("timeout 3s ./sat$options sat.cnf");
if($result)
	echo "<p>" . str_replace("\n", "</p><p>", $result) . "</p>";
else
	echo "<p>Error: time out.</p>";
?>
<?php } ?>
			</div>
		</div>
		<footer>
			<p><i class="far fa-copyright"></i> 2018 <a href="http://jariaza.es" title="jariaza.es" target="_blank">José Antonio Riaza Valverde</a> | <i class="fas fa-university"></i> <a href="http://uclm.es" target="_blank">University of Castilla-La Mancha</a>, <a href="dectau.uclm.es">DEC-Tau</a> research group</p>
			<p><i class="fas fa-balance-scale"></i> Released under the <a href="https://github.com/jariazavalverde/sat/blob/master/LICENSE" target="_blank">BSD-3 Clause license</a> | <i class="fas fa-font"></i> Uses <a href="https://fonts.google.com/specimen/Open+Sans" target="_blank" rel="nofollow">Open Sans font</a> and <a href="https://fontawesome.com/" target="_blank" rel="nofollow">Font Awesome</a></p>
			<p><i class="fas fa-link"></i> <a href="http://jariaza.es" title="jariaza.es" target="_blank">jariaza.es</a> | <i class="fab fa-twitter"></i> <a href="https://twitter.com/jariazavalverde" title="Twitter" target="_blank">@jariazavalverde</a> | <i class="fab fa-github"></i> <a href="https://github.com/jariazavalverde/sat" title="GitHub" target="_blank">jariazavalverde/sat</a></p>
		</footer>
	</body>
</html>
