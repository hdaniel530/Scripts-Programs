/**
 * This program compares benchmarks for Selection Sort, Insertion Sort, Mergesort and Quicksort
 */

import java.awt.Color;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JComboBox;
import java.util.Random;
import java.util.Arrays;

public class Benchmarks extends JFrame
{
  private static int numberOfRuns = 20;

  private JTextField arraySizeInput, display;
  private String sortMethodNames[] =
     {"Selection Sort", "Insertion Sort", "Mergesort", "Quicksort"};
  private JComboBox<String> chooseSortMethod;
  // Starting Java 7 JComboBox is type specific.  In earlier versions use
  // private JComboBox chooseSortMethod;

  private final long seed;
  private int arraySize;

  // Constructor
  public Benchmarks()
  {
    super("Benchmarks");

    Container c = getContentPane();
    c.setLayout(new GridLayout(6, 1));

    c.add(new JLabel(" Array size: "));
    arraySizeInput = new JTextField(4);
    arraySizeInput.setText("1000");
    arraySizeInput.selectAll();
    c.add(arraySizeInput);

    chooseSortMethod = new JComboBox<String>(sortMethodNames);
    // Before Java 7: chooseSortMethod = new JComboBox(sortMethodNames);
    
    c.add(chooseSortMethod);

    JButton run = new JButton("Run");
    run.addActionListener(new RunButtonListener());
    c.add(run);

    c.add(new JLabel(" Avg Time (milliseconds): "));

    display = new JTextField("   Ready");
    display.setBackground(Color.YELLOW);
    display.setEditable(false);
    c.add(display);

    // Use the same random number generator seed for all benchmarks
    //   in one run of this program:
    seed = System.currentTimeMillis();
  }

  /**
   * Fills a[] with random numbers and sorts it using the sorting method
   * specified in sortMethod:
   *    1 -- Selection Sort
   *    2 -- Insertion Sort
   *    3 -- Mergesort
   *    4 -- Quicksort
   * This is repeated numberOfRuns times for better accuracy
   * Returns the total time it took in milliseconds.
   */
  private long runSort(double[] a, int sortMethod, int numberOfRuns)
  {

    _____________________________________________________
    ...
    }

    return totalTime;
  }

  /**
   * Handles Run button events
   */
  private class RunButtonListener implements ActionListener
  {
    public void actionPerformed(ActionEvent e)
    {
      String inputStr = arraySizeInput.getText().trim();
      try
      {
        arraySize = Integer.parseInt(inputStr);
      }
      catch (NumberFormatException ex)
      {
        display.setText(" Invalid array size");
        arraySize = 0;
        return;
      }

      if (arraySize <= 0)
      {
        display.setText(" Invalid array size");
        return;
      }

      if (arraySize <= 0)
        return;

      int sortMethod = chooseSortMethod.getSelectedIndex() + 1;
      double a[] = new double[arraySize];
      double avgTime = (double)runSort(a, sortMethod, numberOfRuns)
                                                          / numberOfRuns;
      display.setText(String.format("  %.2f", avgTime));

      arraySizeInput.selectAll();
      arraySizeInput.requestFocus();
      System.out.println("Array size = " + arraySize +
            " Runs = " + numberOfRuns + " " +
            sortMethodNames[sortMethod - 1] + " avg time: " + avgTime);

    }
  }

  //************************************************************

  public static void main(String[] args)
  {
    numberOfRuns = 20;
    if (args.length > 0)
    {
      int n = -1;
      try
      {
        n = Integer.parseInt(args[0].trim());
      }
      catch (NumberFormatException ex)
      {
        System.out.println("Invalid command-line parameter");
        System.exit(1);
      }
      if (n > 0)
        numberOfRuns = n;
    }

    Benchmarks window = new Benchmarks();
    window.setBounds(300, 300, 180, 200);
    window.setDefaultCloseOperation(EXIT_ON_CLOSE);
    window.setVisible(true);
  }
}



public class InsertionSort
{
  // Sorts a[0], ..., a[a.length-1] in ascending order
  //   using Insertion Sort.
  public static void sort(double[] a)
  {
    for (int n = 1; n < a.length; n++)
    {
      // Save the next element to be inserted:
      double aTemp = a[n];

      // Going backward from a[n-1], shift elements to the
      //   right until you find an element a[i] <= aTemp:

      int i = n;
      while (i > 0 && aTemp < a[i-1])
      {
        a[i] = a[i-1];
        i--;
      }

      // Insert the saved element into a[i]:
      a[i] = aTemp;

      // Increment n (accomplished by n++ in the for loop).
    }
  }
}



public class Mergesort
{
  private static double[] temp;

  // Sorts a[0], ..., a[a.length-1] in ascending order
  //   using the Mergesort algorithm.
  public static void sort(double[] a)
  {
    int n = a.length;
    temp = new double[n];
    recursiveSort(a, 0, n-1);
  }

  // Recursive helper method: sorts a[from], ..., a[to]
  private static void recursiveSort(double[] a, int from, int to)
  {
    if (to - from < 2)       // Base case: 1 or 2 elements
    {
      if (to > from && a[to] < a[from])
      {
        // swap a[to] and a[from]
        double aTemp = a[to]; a[to] = a[from]; a[from] = aTemp;
      }
    }
    else                     // Recursive case
    {
      int middle = (from + to) / 2;
      recursiveSort(a, from, middle);
      recursiveSort(a, middle + 1, to);
      merge(a, from, middle, to);
    }
  }

  // Merges a[from] ... a[middle] and a[middle+1] ... a[to]
  //   into one sorted array a[from] ... a[to]
  private static void merge(double[] a, int from, int middle, int to)
  {
    int i = from, j = middle + 1, k = from;

    // While both arrays have elements left unprocessed:
    while (i <= middle && j <= to)
    {
      if (a[i] < a[j])
      {
        temp[k] = a[i];   // Or simply temp[k] = a[i++];
        i++;
      }
      else
      {
        temp[k] = a[j];
        j++;
      }
      k++;
    }

    // Copy the tail of the first half, if any, into temp:
    while (i <= middle)
    {
      temp[k] = a[i];     // Or simply temp[k++] = a[i++]
      i++;
      k++;
    }

    // Copy the tail of the second half, if any, into temp:
    while (j <= to)
    {
      temp[k] = a[j];     // Or simply temp[k++] = a[j++]
      j++;
      k++;
    }

    // Copy temp back into a
    for (k = from; k <= to; k++)
      a[k] = temp[k];
  }
}



public class Quicksort
{
  // Sorts a[0], ..., a[a.length-1] in ascending order
  //   using the Quicksort algorithm.
  public static void sort(double[] a)
  {
    recursiveSort(a, 0, a.length - 1);
  }

  // Recursive helper method: sorts a[from], ..., a[to]
  private static void recursiveSort(double[] a, int from, int to)
  {
    if (from >= to)
      return;

    // Choose pivot a[p]:
    int p = (from + to ) / 2;
      // The choice of the pivot location may vary:
      //   you can also use p = from or p = to or use
      //   a fancier method, say, the median of the above three.

    // Partition:
    int i = from;
    int j = to;
    while (i <= j)
    {
      if (a[i] <= a[p])
        i++;
      else if (a[j] >= a[p])
        j--;
      else
      {
        swap (a, i, j);
        i++;
        j--;
      }
    }

    // Finish partitioning:
    if (p < j)    // place the pivot in its correct position
    {
      swap (a, j, p);
      p = j;
    }
    else if (p > i)
    {
      swap (a, i, p);
      p = i;
    }

    // Sort recursively:
    recursiveSort(a, from, p - 1);
    recursiveSort(a, p + 1, to);
  }

  private static void swap (double[] a, int i, int j)
  {
    double temp = a[i]; a[i] = a[j]; a[j] = temp;
  }
}



public class SelectionSort
{
  // Sorts a[0], ..., a[a.length-1] in ascending order
  //   using Selection Sort.
  public static void sort(double[] a)
  {
    for (int n = a.length; n > 1; n--)
    {
      // Find the index iMax of the largest element
      //   among a[0], ..., a[n-1]:

      int iMax = 0;
      for (int i = 1; i < n; i++)
      {
        if (a[i] > a[iMax])
          iMax = i;
      }

      // Swap a[iMax] with a[n-1]:

      double aTemp = a[iMax];
      a[iMax] = a[n-1];
      a[n-1] = aTemp;

      // Decrement n (accomplished by n-- in the for loop).
    }
  }
}
