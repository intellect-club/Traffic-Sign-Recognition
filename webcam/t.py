import cv2 
import serial  # For Arduino communication
import time
from ultralytics import YOLO           # library we get yolo from

# Set up Serial communication with Arduino
arduino = serial.Serial(port="COM13", baudrate=9600, timeout=1)  # Change COM port based on your setup

time.sleep(2)  # Allow Arduino to initialize

# Load the trained YOLOv8 model
model = YOLO("best.pt")  

# IP Webcam URL (replace with your actual phone IP and port)
ip_webcam_url = "http://192.168.210.160:8080/video"  # Replace with your IP Webcam URL

# Open the MJPEG stream
cap = cv2.VideoCapture(ip_webcam_url)

if not cap.isOpened():
    print("❌ Error: Could not open MJPEG stream.")
    exit()

# Get class names from YOLO model
class_names = model.names

# Loop through the stream and process frames
while True:
    ret, frame = cap.read()
    if not ret:
        print("❌ Failed to grab frame")
        break

    # Perform YOLOv8 inference
    results = model.predict(frame, conf=0.5)  # Adjust confidence threshold if needed

    # Extract predictions
    detections = results[0].boxes  # Get detected bounding boxes

    for box in detections:
        # Get bounding box coordinates, confidence, and class ID
        x1, y1, x2, y2 = map(int, box.xyxy[0])
        confidence = box.conf[0].item()
        class_id = int(box.cls[0])

        # Get class label
        class_name = class_names[class_id] if class_id in class_names else "Unknown"

        # Draw bounding box
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
       
        # Draw label and confidence
        label = f"{class_name} ({confidence:.2f})"
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        # Send the detected sign to Arduino
        if class_name in ["Stop", "Speed Limit 20","Speed Limit 30", "Speed Limit 40", "Speed Limit 60","Speed Limit 70", "Speed Limit 80","Speed Limit 90", "Speed Limit 100"]:
            arduino.write((class_name + "\n").encode())  # Send sign name to Arduino
            print(f"📡 Sent to Arduino: {class_name}")

    # Display the frame with detections
    cv2.imshow("YOLOv8 Detection", frame)

    # Press 'q' to exit
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# Release resources
cap.release()
cv2.destroyAllWindows()
arduino.close()  # Close the serial connection
