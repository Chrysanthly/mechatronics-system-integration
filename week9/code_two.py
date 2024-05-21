import cv2
import numpy as np


def Color_detection(frame):
    b, g, r = cv2.split(frame)

    bm = np.mean(b)
    gm = np.mean(g)
    rm = np.mean(r)

    if max(bm, gm, rm) == bm:
        return "Blue"
    elif max(bm, gm, rm) == gm:
        return "Green"
    else:
        return "Red"


def main():
    vid = cv2.VideoCapture(0)

    while True:
        _, frame = vid.read()

        color = Color_detection(frame)

        cv2.putText(frame, "Detected Color: " + color, (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    vid.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()