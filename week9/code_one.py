import cv2
import numpy as np

def Color_detection(frame):
    # Convert the frame to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Define the color ranges for red, green, and blue in HSV color space
    red_lower = np.array([0, 120, 70])
    red_upper = np.array([10, 255, 255])
    green_lower = np.array([36, 100, 100])
    green_upper = np.array([86, 255, 255])
    blue_lower = np.array([94, 80, 2])
    blue_upper = np.array([126, 255, 255])
    
    # Create masks for each color
    red_mask = cv2.inRange(hsv, red_lower, red_upper)
    green_mask = cv2.inRange(hsv, green_lower, green_upper)
    blue_mask = cv2.inRange(hsv, blue_lower, blue_upper)
    
    # Find contours for each mask
    contours_red, _ = cv2.findContours(red_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours_green, _ = cv2.findContours(green_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours_blue, _ = cv2.findContours(blue_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    return contours_red, contours_green, contours_blue

def draw_detected_objects(frame, contours, color_name, color_bgr):
    # Sort contours by area (largest first) and limit to the top 3
    contours = sorted(contours, key=cv2.contourArea, reverse=True)[:1]
    
    for contour in contours:
        x, y, w, h = cv2.boundingRect(contour)
        size = max(w, h)
        cv2.rectangle(frame, (x, y), (x + size, y + size), color_bgr, 2)
        cv2.putText(frame, color_name, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, color_bgr, 2)

def main():
    vid = cv2.VideoCapture(0)

    while True:
        ret, frame = vid.read()
        if not ret:
            break

        contours_red, contours_green, contours_blue = Color_detection(frame)

        # Draw the detected red objects
        draw_detected_objects(frame, contours_red, "Red", (0, 0, 255))
        # Draw the detected green objects
        draw_detected_objects(frame, contours_green, "Green", (0, 255, 0))
        # Draw the detected blue objects
        draw_detected_objects(frame, contours_blue, "Blue", (255, 0, 0))

        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    vid.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
