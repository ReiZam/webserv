Bonjour, <?php echo htmlspecialchars($_GET['nom']); ?>.
Tu as <?php echo (int)$_GET['age']; ?> ans.

Bonjour POST, <?php echo htmlspecialchars($_POST['nom']); ?>.
Tu as POST <?php echo (int)$_POST['age']; ?> ans.