import numpy as np
import matplotlib.pyplot as plt
from tensorflow.keras.datasets import mnist
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Input
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.model_selection import train_test_split

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_train = x_train.astype("float32")/255.0
x_test = x_test.astype("float32")/255.0

x_train = x_train.reshape(-1, 28, 28, 1)
x_test = x_test.reshape(-1, 28, 28, 1)

X_train, X_val, y_train, y_val = train_test_split(x_train, y_train, test_size = 0.10, random_state = 42, shuffle = True)
print(len(X_val))

model = Sequential([
    Input(shape=(28, 28, 1)),
    Conv2D(32, (3, 3), activation='relu'),
    MaxPooling2D((3, 3), strides=(1, 1)),
    Conv2D(64, (3, 3), activation='relu'),
    MaxPooling2D((3, 3), strides=(1, 1)),
    Flatten(),
    Dense(64, activation='relu'),
    Dense(10, activation='softmax')
])
model.summary()

model.compile(
    optimizer='adam', 
    loss='sparse_categorical_crossentropy', 
    metrics=['accuracy']
)

history = model.fit(
    X_train, 
    y_train, 
    epochs=3, 
    batch_size=64,
    validation_data = (X_val, y_val),
    verbose=1
) 

print(history.history.keys())

test_loss, test_acc = model.evaluate(x_test, y_test, verbose=0)
print(f"\nTest Loss: {test_loss:.4f} | Test Accuracy: {test_acc*100:.2f}%")

# (Loss & Accuracy)
plt.figure(figsize=(10, 4))
plt.subplot(1, 2, 1)
plt.plot(history.history['loss'], label="Training Loss"),
plt.plot(history.history['val_loss'], label="Validation Loss"),
plt.title("Training Loss VS Validation Loss")
plt.xlabel("Epoches"),
plt.ylabel("Loss"),
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(history.history['accuracy'], label="Training Accuracy"),
plt.plot(history.history['val_accuracy'], label="Validation Accuracy"),
plt.xlabel("Training Acuracy"),
plt.ylabel("Validation Accuracy"),
plt.legend()
plt.show() 


# Display the numbers
indices = np.random.choice(len(x_test), 10, replace=False)

for i, idx in enumerate(indices):
  image = x_test[idx].reshape(1, 28, 28, 1)
  predict = np. argmax(model.predict(image, verbose=0))

  plt.subplot(2, 5, i+1)
  plt.imshow(x_test[idx].reshape(28,28), cmap='gray')
  plt.title(f"True: {y_test[idx]}\nPred: {predict}")
  plt.axis('off')
plt.tight_layout()
plt.show()

# Display Even numbers only
plt.figure(figsize=(10, 2))
odd_indices = np.where(y_test %2 == 0)[0]
indices = np.random.choice(odd_indices, 10, replace=False)

for i, idx in enumerate(indices):
  image = x_test[idx].reshape(1, 28, 28, 1)
  predict = np. argmax(model.predict(image, verbose=0))

  plt.subplot(2, 5, i+1)
  plt.imshow(x_test[idx].reshape(28,28), cmap='gray')
  plt.title(f"True: {y_test[idx]}\nPred: {predict}")
  plt.axis('off')
plt.tight_layout()
plt.show()

# Display Odd numbers only
odd_indices = np.where(y_test %2 != 0)[0]
indices = np.random.choice(odd_indices, 10, replace=False)

for i, idx in enumerate(indices):
  image = x_test[idx].reshape(1, 28, 28, 1)
  predict = np. argmax(model.predict(image, verbose=0))

  plt.subplot(2, 5, i+1)
  plt.imshow(x_test[idx].reshape(28,28), cmap='gray')
  plt.title(f"True: {y_test[idx]}\nPred: {predict}")
  plt.axis('off')
plt.tight_layout()
plt.show()

