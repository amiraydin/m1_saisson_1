from sklearn.ensemble import RandomForestClassifier 
from sklearn import datasets
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt


iris = datasets.load_iris()
x = iris['data']
y = iris['target']

X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.3)

random_f = RandomForestClassifier()
random_f.fit(X_train, y_train)

# _, ax = plt.subplots()
# scatter = ax.scatter(iris.data[:, 0], iris.data[:, 1], c = iris.target)
# ax.set(xlabel = iris.feature_names[0], ylabel = iris.feature_names[1])
# _ = ax.legend(
#     scatter.legend_elements()[0], iris.target_names, loc = "lower right", title = "Classes"
# )
# x_len = len(x)
# y_len = len(y)

_, ax = plt.subplots()
scatter = ax.scatter(X_train[:, 0], X_train[:, 1], c = y_train)
ax.set(xlabel = "x", ylabel = "y")
_ = ax.legend(
    scatter.legend_elements()[0], iris.target_names, loc = "lower right", title = "Classes"
)

# print(len(y_test))
_, ax = plt.subplots()
scatter = ax.scatter(X_test[:, 0], X_test[:, 1], c = y_test)
ax.set(xlabel = "x", ylabel = "y")
_ = ax.legend(
    scatter.legend_elements()[0], iris.target_names, loc = "lower right", title = "Classes"
)

y_pre = random_f.predict(X_test)
# print(y_pre)
from sklearn.metrics import accuracy_score, f1_score, precision_score, recall_score

accuracy = accuracy_score(y_test, y_pre)
# weighted : Calculer les métriques pour chaque étiquette et trouver leur moyenne pondérée par le support (le nombre d'instances vraies pour chaque étiquette). 
# Cela modifie la "macro" pour tenir compte du déséquilibre des étiquettes ;
# il peut en résulter un score F qui n'est pas compris entre la précision et le rappel.
precise = precision_score(y_test, y_pre, average= 'weighted')
# micro : Calculer les métriques globalement en comptant le total des vrais positifs, des faux négatifs et des faux positifs.
precise_mic = precision_score(y_test, y_pre, average= 'micro')
# macro : Calculer les métriques pour chaque étiquette et trouver leur moyenne non pondérée. Cette méthode ne tient pas compte du déséquilibre des étiquettes.
precise_mac = precision_score(y_test, y_pre, average= 'macro')
rappel = recall_score(y_test, y_pre, average= 'weighted')
f_mesure = f1_score(y_test, y_pre, average= 'weighted')

# print("Accuracy:", accuracy)
# print("Precision:", precise)
# print("Precision micro:", precise_mic)
# print("Precision macro:", precise_mac)
# print("rappel:", rappel)
# print("f_mesure:", f_mesure)
manu_random_f = RandomForestClassifier(n_estimators=100, max_depth=10, random_state=42)
manu_random_f.fit(X_train, y_train)
y_pred_manu = manu_random_f.predict(X_test)

_, ax = plt.subplots()
scatter = ax.scatter(X_test[:, 0], X_test[:, 1], c = y_test)
ax.set(xlabel = "x", ylabel = "y")
_ = ax.legend(
    scatter.legend_elements()[0], iris.target_names, loc = "lower right", title = "Classes"
)

# Exo 2
import torch as t
import torchvision 
import torchvision.transforms as transforms
from sklearn.ensemble import RandomForestClassifier 

# Chargez le jeu de données CIFAR-10
transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
batch_size = 4

trainset = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform)
trainloader = t.utils.data.DataLoader(trainset, batch_size=batch_size, shuffle=True, num_workers=2)

testset = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform)
testloader = t.utils.data.DataLoader(testset, batch_size=batch_size, shuffle=False, num_workers=2)

classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')
transform = torchvision.transforms.Compose([torchvision.transforms.ToTensor(), torchvision.transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

train_dataset = torchvision.datasets.CIFAR10(root='./data', train=True, transform=transform, download=True)
test_dataset = torchvision.datasets.CIFAR10(root='./data', train=False, transform=transform, download=True)

# Définissez les chargeurs de données
batch_size = 10
train_loader = t.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = t.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False)
# Préparez les données pour la forêt aléatoire
n_samples, h, w, d = train_loader.dataset.data.shape
X_train = train_loader.dataset.data.reshape((n_samples, h * w * d))
y_train = train_loader.dataset.targets

n_samples, h, w, d = test_loader.dataset.data.shape
X_test = test_loader.dataset.data.reshape((n_samples, h * w * d))
y_test = test_loader.dataset.targets

# Créez un modèle de forêt aléatoire
random_forest_model = RandomForestClassifier(n_estimators=100, random_state=42)

# Entraînez le modèle
random_forest_model.fit(X_train, y_train)

# Faites des prédictions
y_pred = random_forest_model.predict(X_test)

# Évaluez les performances
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)

