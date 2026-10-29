import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D, Dropout
from tensorflow.keras.applications import MobileNet
from tensorflow.keras.datasets import cifar10
from tensorflow.keras.utils import to_categorical

# Load and preprocess the CIFAR-10 dataset
(x_train, y_train), (x_test, y_test) = cifar10.load_data()

# Normalize pixel values
x_train = x_train.astype('float32') / 255.0
x_test = x_test.astype('float32') / 255.0

# One-hot encode labels
y_train = to_categorical(y_train, 10)
y_test = to_categorical(y_test, 10)

# Define the MobileNet backbone model
def build_model(trainable_backbone=False):
    # Load pre-trained MobileNet without the top layer
    backbone = MobileNet(
        input_shape=(32, 32, 3),
        include_top=False,
        weights='imagenet'
    )
    backbone.trainable = trainable_backbone  # Freeze or unfreeze backbone

    # Build the full model
    model = Sequential([
        backbone,
        GlobalAveragePooling2D(),
        Dropout(0.5),
        Dense(256, activation='relu'),
        Dropout(0.5),
        Dense(10, activation='softmax')  # 10 classes
    ])
    model.compile(optimizer='adam',
                  loss='categorical_crossentropy',
                  metrics=['accuracy'])
    return model

# Step 1: Train with Transfer Learning (Backbone Frozen)
print("Training with Transfer Learning (Backbone Frozen)...")
model_tl = build_model(trainable_backbone=False)
history_tl = model_tl.fit(x_train, y_train, epochs=10, batch_size=64, validation_split=0.2)
test_loss_tl, test_accuracy_tl = model_tl.evaluate(x_test, y_test, verbose=0)
print(f"Transfer Learning Test Accuracy: {test_accuracy_tl * 100:.2f}%")

# Step 2: Train with Transfer Learning + Fine-Tuning (Backbone Unfrozen)
print("Training with Transfer Learning + Fine-Tuning (Backbone Unfrozen)...")
model_ft = build_model(trainable_backbone=True)
history_ft = model_ft.fit(x_train, y_train, epochs=10, batch_size=64, validation_split=0.2)
test_loss_ft, test_accuracy_ft = model_ft.evaluate(x_test, y_test, verbose=0)
print(f"Transfer Learning + Fine-Tuning Test Accuracy: {test_accuracy_ft * 100:.2f}%")
