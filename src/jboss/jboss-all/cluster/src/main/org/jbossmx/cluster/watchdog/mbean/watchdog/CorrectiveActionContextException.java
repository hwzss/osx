/**
 * JBoss, the OpenSource EJB server
 *
 * Distributable under LGPL license Version 2.1, February 1999.
 * See terms of license at gnu.org.
 */

package org.jbossmx.cluster.watchdog.mbean.watchdog;

/**
 * Class thrown for CorrectiveActionContext exceptions.
 *
 * @author Stacy Curl
 */
public class CorrectiveActionContextException
    extends Exception
{
    /**
     * Constructor for CorrectiveActionContextException
     *
     * @param    reason the reason for the exception
     */
    public CorrectiveActionContextException(String reason)
    {
        m_reason = reason;
    }

    /**
     * Returns a string representation of this CorrectiveActionContextException
     *
     * @return a string representation of this CorrectiveActionContextException
     */
    public String toString()
    {
        return CorrectiveActionContextException.class.getName() + " : " + m_reason;
    }

    /** the reason for the exception */
    private String m_reason;
}
