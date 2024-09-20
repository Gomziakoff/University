import java.applet.Applet;
import java.awt.Button;
import java.awt.Graphics;

public class SimpleApplet extends Applet {
    Button button;

    public void init() {
        button = new Button("Click me");
        add(button);
    }

    public void paint(Graphics g) {
        g.drawString("Welcome to Java Applet!", 20, 20);
    }
}
