import unittest
import test_utilities

class Tests(unittest.TestCase):
	
  def testVectorPart1(self):
    return_value = test_utilities.runIt('1')
    if return_value != None:
        self.fail(return_value)

  def testVectorPart2(self):
    return_value = test_utilities.runIt('2')
    if return_value != None:
        self.fail(return_value)

  def testVectorPart3(self):
    return_value = test_utilities.runIt('1 E')
    if return_value != None:
        self.fail(return_value)

  def testVectorPart4(self):
    return_value = test_utilities.runIt('2 E')
    if return_value != None:
        self.fail(return_value)

unittest.main()
