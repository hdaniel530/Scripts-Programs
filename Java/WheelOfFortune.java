import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import javax.swing.*;
import javax.swing.JPanel;

public class WheelOfFortune {

    private static final int N = 1;
    public static int[] points = {1000, 500, -1000, 1500, 2000, -2500, 3000, -500, 0, -1000, -500, -1500}; 

    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            JFrame frame = new JFrame();
            frame.setLayout(new GridLayout(N, N, N, N));
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.add(new RotatePanel());
           
            frame.pack();
            frame.setVisible(true);
            System.out.println();
        });
    }
}

class RotatePanel extends JPanel implements ActionListener {

    private static final int SIZE = 800;
    private  Timer timer = new Timer(0, this);
    private Image image = RotatableImage.getImage(SIZE);
    private Image triangle = TrianglePointer.getTriangle(SIZE/3);
    private double dt = Math.PI/100;
    private double theta = 0;
    private int sum = 0;
    int currentPoints = 0;
    private boolean showTitleScreen = true;
    private boolean playing = false;
    private boolean gameOver = false;

    public RotatePanel()
    {
        this.setBackground(Color.black);
        this.setPreferredSize(new Dimension(SIZE, SIZE));
        this.addMouseListener(new MouseAdapter()
        {
            @Override
            public void mousePressed(MouseEvent e) {
              if(showTitleScreen)
              {
                showTitleScreen = false;
                playing = true;
                timer.start();
              }
              
              else if(playing)
              {
                if (dt > 0)
                {	
                	dt = 0;
                	System.out.println((((long)((theta / Math.PI) * 180)) % 360));
                	int index = (int)(((389 - (long)((theta / Math.PI) * 180) % 360)/30) % 12);
                	currentPoints = WheelOfFortune.points[index];
                	if(currentPoints == 0)
                	{
                		playing = false;
                		gameOver = true;
                	}
                	System.out.println(WheelOfFortune.points[index]);
                	sum += currentPoints;
                	if(sum < 0)
                	{
                		playing = false;
                		gameOver = true;
                	}
                }
                else
                  dt = Math.PI/100;
              }
              else
              {
            	  timer.stop();
            	  showTitleScreen = true;
            	  gameOver = false;
            	  sum = 0;
            	  currentPoints = 0;
            	  theta = 0;
            	  dt = Math.PI/100;
              }
          }
        }); 
    }

    @Override
    public void paintComponent(Graphics g)
    {
          super.paintComponent(g);
          g.setColor(Color.WHITE);

          if (showTitleScreen)
          {
            g.setFont(new Font(Font.DIALOG, Font.BOLD, 48));
            g.drawString("WHEEL OF FORTUNE", 160, 100);
            g.setFont(new Font(Font.DIALOG, Font.BOLD, 26));
            g.drawString("CLICK TO PLAY", 300, 400);
          }
          
          else if(playing){
            Graphics2D g2d = (Graphics2D) g; 
            g2d.translate((SIZE-20)/2, (SIZE-20)/2);
            g2d.rotate(theta);
            g2d.drawImage(image, -(SIZE-20)/2, -(SIZE-20)/2, null);
            g2d.rotate(-theta);
            g2d.drawImage(triangle, -60,-(SIZE-10)/2, null);
            g2d.drawString("Last points was: " + Integer.toString(currentPoints), 200, 375);
            g2d.drawString("Total number of points is: " + Integer.toString(sum), 200, 400);
          }
          
          else 
          {
            g.setFont(new Font(Font.DIALOG, Font.BOLD, 48));
            g.drawString("GAME OVER", 250, 100);
            g.setFont(new Font(Font.DIALOG, Font.BOLD, 26));
            g.drawString("CLICK TO REPLAY", 280, 400);
          }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        theta += dt;
        repaint();
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(SIZE, SIZE);
    }

}

class RotatableImage
{
	static public Image getImage(int size)
	{
      size -= 20;
        BufferedImage bi = new BufferedImage(
                size, size, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = bi.createGraphics();
        g2d.setRenderingHint(
                RenderingHints.KEY_ANTIALIASING,
                RenderingHints.VALUE_ANTIALIAS_ON);
        
        final Color outline = Color.white;
        Color[] colors = {new Color((float)0.0, (float)1.0, (float)0.0), 
                new Color((float)0.0, (float)0.0, (float)0.9),
                new Color((float)0.9, (float)0.1, (float)0.7), 
                new Color((float)1.0, (float)0.0, (float)0.1),
                new Color((float)1.0, (float)0.4, (float)0.0),
                new Color((float)1.0, (float)1.0, (float)0.0),
                new Color((float)0.0, (float)0.5, (float)0.0), 
                new Color((float)0.0, (float)0.0, (float)0.7), 
                new Color((float)1.0, (float)0.1, (float)0.7), 
                new Color((float)0.7, (float)0.0, (float)0.1),
                new Color((float)0.9, (float)0.2, (float)0.0),
                new Color((float)0.7, (float)0.6, (float)0.5)};

        //outline
        g2d.setPaint(outline);
        g2d.setStroke(new BasicStroke(5f));
        g2d.draw(new Ellipse2D.Double(0, 0, size, size));
        
        //wheel
        for(int i = 0; i < 12; i++)
        {
        	g2d.setPaint(colors[i]);
        	g2d.fillArc(0, 0, size, size, i*30, 30);
        }
       
        //insert text
        g2d.setFont(g2d.getFont().deriveFont(30f));
        g2d.setPaint(Color.BLACK);
        g2d.drawString(Integer.toString(WheelOfFortune.points[0]), size/2 - 100, 120);
        g2d.drawString(Integer.toString(WheelOfFortune.points[1]), size/2 + 30, 120);
        g2d.drawString(Integer.toString(WheelOfFortune.points[2]), size/2 + 160, 210);
        g2d.drawString(Integer.toString(WheelOfFortune.points[3]), size/2 + 210, 360);
        g2d.drawString(Integer.toString(WheelOfFortune.points[4]), size/2 + 210, 480);
        g2d.drawString(Integer.toString(WheelOfFortune.points[5]), size/2 + 160, 630);
        g2d.drawString(Integer.toString(WheelOfFortune.points[6]), size/2 + 30, 690);
        g2d.drawString(Integer.toString(WheelOfFortune.points[7]), size/2 - 100, 690);
        g2d.drawString("Bankrupt", size/2 - 260, 600);
        g2d.drawString(Integer.toString(WheelOfFortune.points[9]), size/2 - 300, 480);
        g2d.drawString(Integer.toString(WheelOfFortune.points[10]), size/2 - 290, 340);
        g2d.drawString(Integer.toString(WheelOfFortune.points[11]), size/2 - 260, 210);

        g2d.dispose();
        return bi;
    }
}

class TrianglePointer
{
  static public Image getTriangle(int size)
  {
    BufferedImage bi = new BufferedImage(
        size, size, BufferedImage.TYPE_INT_ARGB);
    Graphics2D g2d = bi.createGraphics();
    g2d.setRenderingHint(
        RenderingHints.KEY_ANTIALIASING,
        RenderingHints.VALUE_ANTIALIAS_ON);
    g2d.fillArc(-70, 0, size, size*2/3, 75, 30);
    g2d.dispose();
    return bi;
  }
}
