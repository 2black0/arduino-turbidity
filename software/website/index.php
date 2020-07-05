<!doctype html>
<html lang="en">

<head>
	<!-- Required meta tags -->
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

	<!-- Bootstrap CSS -->
	<link rel="stylesheet" href="css/bootstrap.min.css">


	<title>Turbidity WeMoS D1 Mini</title>
</head>

<body>

  <?php
    include_once("dbconnect.php");
  ?>

	<nav class="navbar navbar-expand-lg navbar-dark bg-dark">
		<div class="container">
			<a class="navbar-brand" href="index.php">Dashboard</a>
			<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNavAltMarkup"
				aria-controls="navbarNavAltMarkup" aria-expanded="false" aria-label="Toggle navigation">
				<span class="navbar-toggler-icon"></span>
			</button>
			<div class="collapse navbar-collapse" id="navbarNavAltMarkup">
				<div class="navbar-nav">
					<a class="nav-item nav-link active" href="index.php">Home <span class="sr-only">(current)</span></a>
					<a class="nav-item nav-link" href="about.php">About</a>
				</div>
			</div>
		</div>
	</nav>


	<div class="container">
		<div class="row text-center">
			<div class="col-md">
				<h3>Grafik</h3>
			</div>

			<div class="col-md">
				<h3>Tabel</h3>
			</div>
		</div>

		<div class="row text-center">
			<div class="col-md">
				<!--<img src="img/1.jpg" class="img-fluid" alt="Responsive image">-->
				<canvas id="myChart" width="200" height="150"></canvas>
			</div>

			<div class="col-md">
				<!--<img src="img/2.jpg" class="img-fluid" alt="Responsive image">-->
				<table class="table">
					<thead class="thead-dark">
						<tr>
							<th scope="col">No</th>
							<th scope="col">Waktu</th>
							<th scope="col">Turbidity</th>
							<th scope="col">Status</th>
						</tr>
					</thead>
            <?php
              $hasil = mysqli_query($dbc, "SELECT * FROM logsensor ORDER BY id_data DESC LIMIT 10");
              echo "<tbody>";
              $id = 0;
              $status = "";
              $datalabels = [];
              $valuelabels = [];
              while ($baris = mysqli_fetch_row($hasil)) 
                { 
                  array_push($datalabels, $baris[1]);
                  array_push($valuelabels, $baris[2]);

                  $id         += 1;
                  $id_data    = $baris[0];
                  $waktu      = $baris[1];  
                  $turbidity  = $baris[2];
                  if ($turbidity<100) {
                    $status = "Bersih";
                  }
                  elseif ($turbidity>=100 && $turbidity <= 200) {
                    $status = "Cukup Keruh";
                  }
                  else {
                    $status = "Sangat Keruh";
                  }
                  echo "<tr>";
                  echo "<td>$id</td>";
                  echo "<td>$waktu</td>";
                  echo "<td>$turbidity</td>";
                  echo "<td>$status</td>";
                  echo "</tr>";
                }
            ?>
          </tbody>
        </table>
      </div>
    </div>
	</div>

	<!-- Optional JavaScript -->
	<!-- jQuery first, then Popper.js, then Bootstrap JS -->
	<script src="js/jquery-3.5.1.slim.min.js"></script>
	<script src="js/popper.min.js"></script>
	<script src="js/bootstrap.bundle.min.js"></script>
	<script src="js/Chart.js"></script>

</body>

<footer>
	<script>
    var ctx = document.getElementById("myChart").getContext('2d');

    var datalabels = [
      <?php 
        $sql = "SELECT * FROM logsensor";
        $hasil = mysqli_query($dbc, $sql);
        $lastrow = mysqli_num_rows($hasil);
        $lastrow = $lastrow-10;

        $sql = "SELECT * FROM logsensor ORDER BY id_data ASC LIMIT $lastrow,10";
        $hasil = mysqli_query($dbc, $sql);
        $x = 0;
        while ($baris = mysqli_fetch_row($hasil))
        { 
          $x += 1;
          //echo "\"";
          echo "\"$baris[1]\"";
          if ($x <= 9) {
            echo ",";
          }
        }
			?>
    ];

    var valuelabels = [
      <?php 
        $sql = "SELECT * FROM logsensor";
        $hasil = mysqli_query($dbc, $sql);
        $lastrow = mysqli_num_rows($hasil);
        $lastrow = $lastrow-10;

        $sql = "SELECT * FROM logsensor ORDER BY id_data ASC LIMIT $lastrow,10";
        $hasil = mysqli_query($dbc, $sql);
        $x = 0;
        while ($baris = mysqli_fetch_row($hasil))
        {
          $x += 1;
          echo "$baris[2]";
          if ($x <= 9) {
            echo ",";
          }
        }
			?>
    ];

    //var datalabels = ["13:00", "13:05", "13:10", "13:15", "13:20", "13:25", "13:30", "13:35", "13:40", "13:45"];
    //var valuelabels = [4, 65, 55, 99, 0, 22, 21, 11, 56, 78];

		var myChart = new Chart(ctx, {
			type: 'line',
			data: {
        labels: datalabels,
				datasets: [{
					label: 'Turbidity (NTU)',
					data: valuelabels,
					fill: false,
					borderColor: 'rgba(0,0,0,0.5)',
					borderWidth: 2
				}]
			},
			options: {
				scales: {
					yAxes: [{
						ticks: {
							beginAtZero: true
						}
					}]
				}
			}
		});
	</script>
</footer>

</html>