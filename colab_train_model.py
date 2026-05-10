import pandas as pd
import joblib

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix

HABITAT_TARGETS = {
    "ideal_temperature_c": "19-23",
    "safe_temperature_c": "16-28",
    "ideal_humidity_percent": "70-85",
    "safe_humidity_percent": "60-90",
    "basis": "Cool, moist Kodaikanal shola habitat inferred from published E. blascoi habitat records and local climate data.",
}

df = pd.read_csv("elaeocarpus_blascoi_synthetic_sensor_dataset_50000.csv")

features = [
    "temperature_c",
    "humidity_percent",
    "soil_moisture_digital",
    "pump_duration_sec",
    "watering_event",
]
target = "survival_condition"

X = df[features]
y = df[target]

label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

X_train, X_test, y_train, y_test = train_test_split(
    X,
    y_encoded,
    test_size=0.2,
    random_state=42,
    stratify=y_encoded,
)

model = RandomForestClassifier(
    n_estimators=100,
    random_state=42,
    class_weight="balanced",
)
model.fit(X_train, y_train)

y_pred = model.predict(X_test)

print("Accuracy:", accuracy_score(y_test, y_pred))
print(classification_report(y_test, y_pred, target_names=label_encoder.classes_))
print("Confusion matrix:")
print(confusion_matrix(y_test, y_pred))
print("Prototype habitat targets:")
for key, value in HABITAT_TARGETS.items():
    print(f"{key}: {value}")

joblib.dump(model, "elaeocarpus_survival_model.pkl")
joblib.dump(label_encoder, "label_encoder.pkl")

print("Saved elaeocarpus_survival_model.pkl and label_encoder.pkl")
