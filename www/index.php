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
		<div class="container">
			<header class="container-header">
			<h1><i class="fas fa-hashtag"></i> Boolean satisfiability problem</h1>
			</header>
			<div class="columns">
				<div class="column-left"><div>
					<form action="./" method="POST">
						<textarea id="problem" name="problem" placeholder="Your SAT program here..."><?php
if(isset($_GET["sample"]))
	if(file_exists("sample/" . $_GET["sample"]))
		echo file_get_contents("sample/" . $_GET["sample"]);
?></textarea>
						<button class="button" type="submit">
							<i class="fas fa-cogs"></i> &nbsp; Solve it!
						</button>
					</form>
				</div></div>
				<div class="column-right"><div>
					<h2><i class="fas fa-flask"></i> Examples</h2>
						<ul class="samples-list">
<?php
foreach(scandir("sample") as $f)
	if($f != "." && $f != "..")
		echo "\t\t\t\t\t\t\t<li><a href=\"./?sample=$f\"><i class=\"fas fa-file\"></i> <span>$f</span></a></li>\n";
?>
						</ul>
				</div></div>
			</div>
		</div>
		<footer>
			<p><i class="far fa-copyright"></i> 2018 <a href="http://jariaza.es" target="_blank">José Antonio Riaza Valverde</a> | <i class="fas fa-university"></i> <a href="http://uclm.es" target="_blank">University of Castilla-La Mancha</a>, <a href="dectau.uclm.es">DEC-Tau</a> research group</p>
			<p><i class="fas fa-balance-scale"></i> Released under the <a href="https://github.com/jariazavalverde/sat/blob/master/LICENSE" target="_blank">BSD-3 Clause license</a> | <i class="fas fa-font"></i> Uses <a href="https://fonts.google.com/specimen/Open+Sans" target="_blank" rel="nofollow">Open Sans font</a> and <a href="https://fontawesome.com/" target="_blank" rel="nofollow">Font Awesome</a></p>
			<p class="big"><i class="fab fa-github"></i> <a href="https://github.com/jariazavalverde/sat" target="_blank">Fork on GitHub!</a></p>
		</footer>
	</body>
</html>
