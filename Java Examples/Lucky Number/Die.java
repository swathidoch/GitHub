/**
 * Create a die of n sides with values 1 - n.
 * 
 * @author Mr. Poland
 * @version 1.0 11/14/12
 */

public class Die 
{
	/**
	 * The fewest number of sides of a die.  Initially set to 4.
	 */
	private final int MIN_FACES = 4;
	
	/**
	 * The number of faces (or sides) of a die.
	 */
	private int numFaces;
	
	/**
	 * The current value of a face (or dies) or a die.
	 */
	private int faceValue;
	

	/**
	 * Defaults to a six-sided die. Initial face value is 1.
	 */
	public Die()
	{
		numFaces = 6;
		faceValue = 1;
	}
	
	
	/**
	 * Explicitly sets the size of the die. Defaults to a size of six
	 * if the parameter is invalid. Initial face value is 1.
	 *
	 * @param faces The number of faces (or sides) of a die.
	 */
	public Die (int faces)
	{
		if (faces < MIN_FACES)
			numFaces = 6;
		else
			numFaces = faces;
		
		faceValue = 1;
	}		
	
		
	/**
	 * Rolls the die and returns the result.
	 *
	 * @return the result of the roll
	 */
	public int roll()
	{
		faceValue = (int)(Math.random() * numFaces) + 1;
		return faceValue;
	}
	
		
	/**
	 * Returns the current die value.
	 * 
	 * @return the current die value.
	 */
	public int getFaceValue()
	{
		return faceValue;
	}
	
	/**
	 * Returns a string of the value of the die.
	 * 
	 * @return the string of the value of the die.
	 */
	public String toString()
	{
		return "" + faceValue;
	}
}
