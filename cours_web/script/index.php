<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="Content-Language" content="en">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/all.min.css" integrity="sha512-MV7K8+y+gLIBoVD59lQIYicR65iaqukzvf/nwasF0nqhPay5w/9lJmVM2hMDcnK1OnMGCdVK+iQrJ7lzPJQd1w==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="chemin.css">
    <title>Script Informations on Directory </title>
</head>
<?php
header('Content-Language: en');
include('./chemin_info.php');
?>

<body>
    <main class="m-2 rounded">
        <nav class="navbar navbar-expand-lg navbar-light bg-light">
            <div class="container fluid">
                <h5 class="nav-item text-center"> Informations on your Directory</h5>
                <div class="collapse navbar-collapse" id="navbarNav">
                    <ul class="navbar-nav">
                        <li class="nav-item ms-3 me-3">
                            <label for="filepicker">Search your directory</label>
                            <input type="file" name="chemin" webkitdirectory multiple id="filepicker" required class="form-control form-control-sm" />
                        </li>
                        <li class="nav-item ms-3 me-5">
                            <label for="manually">Enter the path to your directory</label>
                            <form class="d-flex gap-2" action="" method="post">
                                <input type="text" name="chemin" id="manually" autocomplete="on" required class="form-control col-8 form-control-sm" />
                                <button class="btn btn-outline-primary btn-sm" type="submit">Manually</button>
                            </form>
                        </li>
                        <li class="nav-item ms-5 me-3">
                            <label for="searchInput">Search the name of your folder</label>
                            <input class="form-control bg-gradient form-control-sm" id="searchInput" onkeyup="searchFunc()" type="search" autocomplete="off" placeholder="Search" aria-label="Search">
                            <!-- <label for="searchInput"><span class="sicon_list"><i class="fa-solid fa-magnifying-glass"></i></span></label> -->
                        </li>
                    </ul>
                </div>
            </div>
        </nav>
        <div class="p-3 text-center">
            <h4>Directory Tree View</h4>
        </div>
        <ul id="listing"></ul>
        <div class="main_list mt-4" id="treeView">
            <?php if ($result) : ?>
                <?php
                function generateTreeView($data, $level = 0)
                {
                    $indentation = str_repeat(" ", $level);
                ?>
                    <?php if ($level > 0) : ?>
                        <?= $indentation ?><ul class="nested">
                        <?php endif; ?>
                        <?php foreach ($data as $item) : ?>
                            <li>
                                <?php if (isset($item['NomFich'])) : ?>
                                    <h6 class="nomFichier"><?= $item['NomFich'] ?> <i class="far fa-file me-2"></i> size (<?= $item['Taille'] ?>) </h6>
                                <?php elseif (isset($item['NomDoss'])) : ?>
                                    <span class="caret"><i class="far fa-folder me-2"></i><?= $item['NomDoss'] ?></span>
                                    <?php if (isset($item['Children'])) {
                                        generateTreeView($item['Children'], $level + 1);
                                    } ?>
                                <?php endif; ?>
                            </li>
                        <?php endforeach; ?>
                        <?php if ($level > 0) : ?>
                            <?= $indentation ?>
                        </ul>
                    <?php endif; ?>
                <?php }; ?>
                <?php generateTreeView($result); ?>
            <?php endif; ?>
        </div>
    </main>
    <script>
        // input de search
        // function searchFunc() {
        //     var input, filter, treeView, h6, i, txtValue;
        //     input = document.getElementById("searchInput");
        //     filter = input.value.toUpperCase();
        //     treeView = document.getElementsByClassName("main_list")[0]; // Utilisez la classe pour cibler l'élément
        //     if (treeView) {
        //         h6 = treeView.querySelectorAll("h6"); // Sélectionnez tous les éléments <h6> dans la structure d'arborescence
        //         for (i = 0; i < h6.length; i++) {
        //             txtValue = h6[i].textContent || h6[i].innerText;
        //             if (txtValue.toUpperCase().indexOf(filter) > -1) {
        //                 h6[i].closest('li').style.display = "";
        //             } else {
        //                 h6[i].closest('li').style.display = "none";
        //             }
        //         }
        //     }
        // }
        function searchFunc() {
            var input, filter, table, h6, i, txtValue;
            input = document.getElementById("searchInput");
            filter = input.value.toUpperCase();
            table = document.getElementsByClassName("main_list")[0];
            if (table) {
                h6 = table.querySelectorAll("h6");
                // console.log("h6 length", h6);
                for (i = 0; i < h6.length; i++) {
                    txtValue = h6[i].textContent || h6[i].innerText;
                    // console.log("textValue", txtValue);
                    console.log("filter index of", txtValue.toUpperCase().indexOf(filter) > -1);
                    if (txtValue.toUpperCase().indexOf(filter) > -1) {
                        h6[i].style.display = "";
                    } else {
                        h6[i].style.display = "none";
                    }
                }
            }
        }
        document.getElementById("filepicker").addEventListener(
            "change",
            (event) => {
                let output = document.getElementById("listing");
                let files = event.target.files;
                for (const file of files) {
                    displayFile(output, file, file.webkitRelativePath);
                }
            },
            false,
        );

        function displayFile(parent, file, path, depth = 0) {
            let item = document.createElement("li");
            let isDirectory = file.isDirectory;
            console.log("s directory", isDirectory);
            console.log("s parent", parent);
            console.log("s file", file);
            console.log("s depth", depth);
            let span = document.createElement("span");
            // Add caret class for toggle
            span.classList.add("caret");
            span.textContent = path;
            if (isDirectory) {
                let subList = document.createElement("ul");
                // Add nested class for styling
                subList.classList.add("nested");
                item.appendChild(span);
                item.appendChild(subList);
                if (file && isDirectory) {
                    let directoryReader = file.createReader();
                    directoryReader.readEntries((entries) => {
                        for (const entry of entries) {
                            displayFile(subList, entry, entry.name, depth + 1);
                        }
                    });
                }
            } else {
                item.appendChild(span);
                item.textContent += " (" + formatBytes(file.size) + ")";
            }
            // Add depth-based indentation
            item.style.marginLeft = depth * 20 + 'px';
            parent.appendChild(item);
        }

        function formatBytes(bytes, decimals = 2) {
            if (bytes === 0) return '0 Bytes';
            const k = 1024;
            const dm = decimals < 0 ? 0 : decimals;
            const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];
            const i = Math.floor(Math.log(bytes) / Math.log(k));
            return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
        }

        var toggler = document.getElementsByClassName("caret");
        var i;

        for (i = 0; i < toggler.length; i++) {
            toggler[i].addEventListener("click", function() {
                this.parentElement.querySelector(".nested").classList.toggle("active");
                this.classList.toggle("caret-down");
            });
        }
    </script>
</body>

</html>